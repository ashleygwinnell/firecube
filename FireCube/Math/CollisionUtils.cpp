#include "CollisionUtils.h"
#include "MathUtils.h"
#include "Physics/CollisionShape.h"
#include "Physics/PhysicsWorld.h"

using namespace FireCube;

void CollisionUtils::SweepEllipsoidMesh(vec3 transformedPosition, vec3 transformedVelocity, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result)
{
	vec3 normalizedTransformedVelocity = transformedVelocity.Normalized();
	for (const auto &tri : collisionMesh.triangles)
	{
		const vec3 p1 = transform * tri.p0;
		const vec3 p2 = transform * tri.p1;
		const vec3 p3 = transform * tri.p2;
		const Plane &trianglePlane = Plane(p1, p2, p3);

		if (trianglePlane.IsFrontFacingTo(normalizedTransformedVelocity))
		{
			float t0, t1;
			bool embeddedInPlane = false;

			float signedDistToTrianglePlane = trianglePlane.GetDistance(transformedPosition);

			float normalDotVelocity = trianglePlane.GetNormal().Dot(transformedVelocity);

			if (normalDotVelocity == 0)
			{
				if (fabs(signedDistToTrianglePlane) >= 1.0f)
				{
					continue;
				}
				else
				{
					embeddedInPlane = true;
					t0 = 0.0f;
					t1 = 1.0f;
				}
			}
			else
			{
				t0 = (-1.0f - signedDistToTrianglePlane) / normalDotVelocity;
				t1 = (1.0f - signedDistToTrianglePlane) / normalDotVelocity;

				if (t0 > t1)
				{
					float temp = t1;
					t1 = t0;
					t0 = temp;
				}

				if ((t0 > 1.0f) || (t1 < 0.0f))
				{
					continue;
				}

				if (t0 < 0.0f) t0 = 0.0f;
				if (t1 < 0.0f) t1 = 0.0f;
				if (t0 > 1.0f) t0 = 1.0f;
				if (t1 > 1.0f) t1 = 1.0f;
			}
			vec3 collisionPoint;
			vec3 nearestNormal = trianglePlane.GetNormal();
			bool foundCollision = false;
			float t = 1.0f;

			if (!embeddedInPlane)
			{
				vec3 planeIntersectionPoint = (transformedPosition - trianglePlane.GetNormal()) + transformedVelocity*t0;
				if (MathUtils::PointTri(p1, p2, p3, planeIntersectionPoint))
				{
					foundCollision = true;
					t = t0;
					collisionPoint = planeIntersectionPoint;
				}

			}

			if (foundCollision == false)
			{
				vec3 tVelocity = transformedVelocity;
				vec3 tPosition = transformedPosition;
				float velocitySquaredLength = tVelocity.Length2();
				float a, b, c;
				float newT;

				a = velocitySquaredLength;

				//P1
				b = 2.0f*(tVelocity.Dot(tPosition - p1));
				c = (p1 - tPosition).Length2() - 1.0f;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p1;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}

				//P2
				b = 2.0f*(tVelocity.Dot(tPosition - p2));
				c = (p2 - tPosition).Length2() - 1.0f;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p2;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}

				//P3
				b = 2.0f*(tVelocity.Dot(tPosition - p3));
				c = (p3 - tPosition).Length2() - 1.0f;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p3;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}


				// p1-p2

				vec3 edge = p2 - p1;
				vec3 posToVertex = p1 - tPosition;
				float edgeSquaredLength = edge.Length2();
				float edgeDotVelocity = edge.Dot(tVelocity);
				float edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(1 - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p1 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

				// p2-p3

				edge = p3 - p2;
				posToVertex = p2 - tPosition;
				edgeSquaredLength = edge.Length2();
				edgeDotVelocity = edge.Dot(tVelocity);
				edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(1 - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p2 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

				// p3-p1

				edge = p1 - p3;
				posToVertex = p3 - tPosition;
				edgeSquaredLength = edge.Length2();
				edgeDotVelocity = edge.Dot(tVelocity);
				edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(1 - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p3 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

			}

			if (foundCollision == true)
			{
				float distToCollision = t*transformedVelocity.Length();
				result.contacts.push_back(CollisionContact());
				CollisionContact &e = result.contacts.back();
				e.time = t;
				e.distance = distToCollision;
				e.intersectionPoint = collisionPoint;
				e.normal = nearestNormal;
				if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
				{
					result.nearestTime = t;
					result.nearestDistance = distToCollision;
					result.nearestIntersectionPoint = collisionPoint;
					result.nearestNormal = nearestNormal;
					result.collisionFound = true;
				}
			}
		}
	}
}

bool CollisionUtils::SweepSphereTriangle(vec3 position, vec3 velocity, float radius, const CollisionTriangle &tri, float &intersectionDistance, vec3 &intersectionPoint, vec3 &intersectionNormal)
{
	const vec3 &p1 = tri.p0;
	const vec3 &p2 = tri.p1;
	const vec3 &p3 = tri.p2;
	const Plane &trianglePlane = Plane(p1, p2, p3);

	if (trianglePlane.IsFrontFacingTo(velocity))
	{
		float t0, t1;
		bool embeddedInPlane = false;

		float signedDistToTrianglePlane = trianglePlane.GetDistance(position);

		float normalDotVelocity = trianglePlane.GetNormal().Dot(velocity);

		if (normalDotVelocity == 0)
		{
			if (fabs(signedDistToTrianglePlane) >= 1.0f)
			{
				return false;
			}
			else
			{
				embeddedInPlane = true;
				t0 = 0.0f;
				t1 = 1.0f;
			}
		}
		else
		{
			t0 = (-radius - signedDistToTrianglePlane) / normalDotVelocity;
			t1 = (radius - signedDistToTrianglePlane) / normalDotVelocity;

			if (t0 > t1)
			{
				float temp = t1;
				t1 = t0;
				t0 = temp;
			}

			if ((t0 > 1.0f) || (t1 < 0.0f))
			{
				return false;
			}

			if (t0 < 0.0f) t0 = 0.0f;
			if (t1 < 0.0f) t1 = 0.0f;
			if (t0 > 1.0f) t0 = 1.0f;
			if (t1 > 1.0f) t1 = 1.0f;
		}
		vec3 collisionPoint;
		vec3 nearestNormal = trianglePlane.GetNormal();
		bool foundCollision = false;
		float t = 1.0f;

		if (!embeddedInPlane)
		{
			vec3 planeIntersectionPoint = (position - trianglePlane.GetNormal()) + velocity*t0;
			if (MathUtils::PointTri(p1, p2, p3, planeIntersectionPoint))
			{
				foundCollision = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}

		}

		if (foundCollision == false)
		{
			vec3 tVelocity = velocity;
			vec3 tPosition = position;
			float velocitySquaredLength = tVelocity.Length2();
			float a, b, c;
			float newT;

			a = velocitySquaredLength;

			//P1
			b = 2.0f*(tVelocity.Dot(tPosition - p1));
			c = (p1 - tPosition).Length2() - radius * radius;
			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p1;
				nearestNormal = (tPosition - collisionPoint).Normalized();
			}

			//P2
			b = 2.0f*(tVelocity.Dot(tPosition - p2));
			c = (p2 - tPosition).Length2() - radius * radius;
			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p2;
				nearestNormal = (tPosition - collisionPoint).Normalized();
			}

			//P3
			b = 2.0f*(tVelocity.Dot(tPosition - p3));
			c = (p3 - tPosition).Length2() - radius * radius;
			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = p3;
				nearestNormal = (tPosition - collisionPoint).Normalized();
			}


			// p1-p2

			vec3 edge = p2 - p1;
			vec3 posToVertex = p1 - tPosition;
			float edgeSquaredLength = edge.Length2();
			float edgeDotVelocity = edge.Dot(tVelocity);
			float edgeDotPosToVertex = edge.Dot(posToVertex);

			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
			c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
				if ((f >= 0.0f) && (f <= 1.0f))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p1 + edge*f;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}
			}

			// p2-p3

			edge = p3 - p2;
			posToVertex = p2 - tPosition;
			edgeSquaredLength = edge.Length2();
			edgeDotVelocity = edge.Dot(tVelocity);
			edgeDotPosToVertex = edge.Dot(posToVertex);

			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
			c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
				if ((f >= 0.0f) && (f <= 1.0f))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p2 + edge*f;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}
			}

			// p3-p1

			edge = p1 - p3;
			posToVertex = p3 - tPosition;
			edgeSquaredLength = edge.Length2();
			edgeDotVelocity = edge.Dot(tVelocity);
			edgeDotPosToVertex = edge.Dot(posToVertex);

			a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
			b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
			c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

			if (MathUtils::GetLowestRoot(a, b, c, t, newT))
			{
				float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
				if ((f >= 0.0f) && (f <= 1.0f))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p3 + edge*f;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}
			}

		}

		if (foundCollision == true)
		{
			float distToCollision = t * velocity.Length();
			intersectionDistance = distToCollision;
			intersectionPoint = collisionPoint;
			intersectionNormal = nearestNormal;
		}

		return foundCollision;
	}

	return false;
}

void CollisionUtils::SweepSphereMesh(vec3 position, vec3 velocity, float radius, const CollisionMesh &collisionMesh, CollisionResult &result)
{
	for (const auto &tri : collisionMesh.triangles)
	{
		const vec3 &p1 = tri.p0;
		const vec3 &p2 = tri.p1;
		const vec3 &p3 = tri.p2;
		const Plane &trianglePlane = Plane(p1, p2, p3);

		if (trianglePlane.IsFrontFacingTo(velocity))
		{
			float t0, t1;
			bool embeddedInPlane = false;

			float signedDistToTrianglePlane = trianglePlane.GetDistance(position);

			float normalDotVelocity = trianglePlane.GetNormal().Dot(velocity);

			if (normalDotVelocity == 0)
			{
				if (fabs(signedDistToTrianglePlane) >= 1.0f)
				{
					continue;
				}
				else
				{
					embeddedInPlane = true;
					t0 = 0.0f;
					t1 = 1.0f;
				}
			}
			else
			{
				t0 = (-radius - signedDistToTrianglePlane) / normalDotVelocity;
				t1 = (radius - signedDistToTrianglePlane) / normalDotVelocity;

				if (t0 > t1)
				{
					float temp = t1;
					t1 = t0;
					t0 = temp;
				}

				if ((t0 > 1.0f) || (t1 < 0.0f))
				{
					continue;
				}

				if (t0 < 0.0f) t0 = 0.0f;
				if (t1 < 0.0f) t1 = 0.0f;
				if (t0 > 1.0f) t0 = 1.0f;
				if (t1 > 1.0f) t1 = 1.0f;
			}
			vec3 collisionPoint;
			vec3 nearestNormal = trianglePlane.GetNormal();
			bool foundCollision = false;
			float t = 1.0f;

			if (!embeddedInPlane)
			{
				vec3 planeIntersectionPoint = (position - trianglePlane.GetNormal()) + velocity*t0;
				if (MathUtils::PointTri(p1, p2, p3, planeIntersectionPoint))
				{
					foundCollision = true;
					t = t0;
					collisionPoint = planeIntersectionPoint;
				}

			}

			if (foundCollision == false)
			{
				vec3 tVelocity = velocity;
				vec3 tPosition = position;
				float velocitySquaredLength = tVelocity.Length2();
				float a, b, c;
				float newT;

				a = velocitySquaredLength;

				//P1
				b = 2.0f*(tVelocity.Dot(tPosition - p1));
				c = (p1 - tPosition).Length2() - radius * radius;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p1;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}

				//P2
				b = 2.0f*(tVelocity.Dot(tPosition - p2));
				c = (p2 - tPosition).Length2() - radius * radius;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p2;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}

				//P3
				b = 2.0f*(tVelocity.Dot(tPosition - p3));
				c = (p3 - tPosition).Length2() - radius * radius;
				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					t = newT;
					foundCollision = true;
					collisionPoint = p3;
					nearestNormal = (tPosition - collisionPoint).Normalized();
				}


				// p1-p2

				vec3 edge = p2 - p1;
				vec3 posToVertex = p1 - tPosition;
				float edgeSquaredLength = edge.Length2();
				float edgeDotVelocity = edge.Dot(tVelocity);
				float edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p1 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

				// p2-p3

				edge = p3 - p2;
				posToVertex = p2 - tPosition;
				edgeSquaredLength = edge.Length2();
				edgeDotVelocity = edge.Dot(tVelocity);
				edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p2 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

				// p3-p1

				edge = p1 - p3;
				posToVertex = p3 - tPosition;
				edgeSquaredLength = edge.Length2();
				edgeDotVelocity = edge.Dot(tVelocity);
				edgeDotPosToVertex = edge.Dot(posToVertex);

				a = edgeSquaredLength*-velocitySquaredLength + edgeDotVelocity*edgeDotVelocity;
				b = edgeSquaredLength*(2 * tVelocity.Dot(posToVertex)) - 2.0f*edgeDotVelocity*edgeDotPosToVertex;
				c = edgeSquaredLength*(radius * radius - posToVertex.Length2()) + edgeDotPosToVertex*edgeDotPosToVertex;

				if (MathUtils::GetLowestRoot(a, b, c, t, newT))
				{
					float f = (edgeDotVelocity*newT - edgeDotPosToVertex) / edgeSquaredLength;
					if ((f >= 0.0f) && (f <= 1.0f))
					{
						t = newT;
						foundCollision = true;
						collisionPoint = p3 + edge*f;
						nearestNormal = (tPosition - collisionPoint).Normalized();
					}
				}

			}

			if (foundCollision == true)
			{
				float distToCollision = t * velocity.Length();
				result.contacts.push_back(CollisionContact());
				CollisionContact &e = result.contacts.back();
				e.time = t;
				e.distance = distToCollision;
				e.intersectionPoint = collisionPoint;
				e.normal = nearestNormal;
				if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
				{
					result.nearestTime = t;
					result.nearestDistance = distToCollision;
					result.nearestIntersectionPoint = collisionPoint;
					result.nearestNormal = nearestNormal;
					result.collisionFound = true;
				}
			}
		}
	}
}

#define ADD_TRI(pp0, pp1, pp2){ \
extrudedTris[extrudedTrianglesCount].p0 = pp0; \
extrudedTris[extrudedTrianglesCount].p1 = pp1; \
extrudedTris[extrudedTrianglesCount].p2 = pp2; \
CalcNormal(pp0, pp1, pp2, extrudedTrisNormals[extrudedTrianglesCount]);	\
extrudedTrianglesCount++;}

#define ADD_TRI_CHECK_DIR(pp0, pp1, pp2, d){ \
auto& tri = extrudedTris[extrudedTrianglesCount]; \
tri.p0 = pp0; \
tri.p1 = pp1; \
tri.p2 = pp2; \
vec3 nrm; \
CalcNormal(pp0, pp1, pp2, nrm);	 \
if(nrm.Dot(d)>0.0f) { \
vec3 tmp = tri.p1; \
tri.p1 = tri.p2; \
tri.p2 = tmp; \
nrm = -nrm; \
} \
extrudedTrisNormals[extrudedTrianglesCount] = nrm; \
extrudedTrianglesCount++; }

inline void CalcNormal(const vec3 &p0, const vec3 &p1, const vec3 &p2, vec3 &normal)
{
	normal = Cross(p1 - p0, p2 - p0);
}

bool CollisionUtils::SweepCapsuleMesh(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const CollisionMesh &collisionMesh, CollisionResult &result)
{
	vec3 center = (capsuleP0 + capsuleP1) * 0.5f;
	vec3 extrusionDir = (capsuleP0 - capsuleP1) * 0.5f;
	float halfHeight = extrusionDir.Length();
	bool mustExtrude = halfHeight != 0.0f;
	if (mustExtrude)
	{
		vec3 capsuleAxis = extrusionDir / halfHeight;
		float colinearity = std::abs(capsuleAxis.Dot(dir));
		mustExtrude = (colinearity < (1.0f - EPSILON));
	}

	if (!mustExtrude)
	{
		vec3 sphereCenter = center + dir * halfHeight;
		SweepSphereMesh(sphereCenter, dir * distance, radius, collisionMesh, result);
		return result.collisionFound;
	}

	CollisionTriangle extrudedTris[7];
	vec3 extrudedTrisNormals[7];

	float nearestDistance = 10e6;
	vec3 nearestTriNormal;
	bool foundCollision = false;
	CollisionTriangle nearestTriangle;
	unsigned int nearestSrcTriangleIndex = std::numeric_limits<unsigned int>::max();

	for (unsigned int i = 0; i < collisionMesh.triangles.size(); ++i)
	{
		auto &tri = collisionMesh.triangles[i];
		unsigned int extrudedTrianglesCount = 0;
		vec3 p0 = tri.p0;
		vec3 p1 = tri.p1;
		vec3 p2 = tri.p2;
		vec3 triNormal;
		CalcNormal(p0, p1, p2, triNormal);

		vec3 p0b = p0 + extrusionDir;
		vec3 p1b = p1 + extrusionDir;
		vec3 p2b = p2 + extrusionDir;

		p0 -= extrusionDir;
		p1 -= extrusionDir;
		p2 -= extrusionDir;

		if (triNormal.Dot(extrusionDir) >= 0.0f)
		{
			ADD_TRI(p0b, p1b, p2b);
		}
		else
		{
			ADD_TRI(p0, p1, p2);
		}

		{
			ADD_TRI_CHECK_DIR(p1, p1b, p2b, dir);
			ADD_TRI_CHECK_DIR(p1, p2b, p2, dir);
		}

		{
			ADD_TRI_CHECK_DIR(p0, p2, p2b, dir);
			ADD_TRI_CHECK_DIR(p0, p2b, p0b, dir);
		}

		{
			ADD_TRI_CHECK_DIR(p0b, p1b, p1, dir);
			ADD_TRI_CHECK_DIR(p0b, p1, p0, dir);
		}

		triNormal.Normalize();

		for (unsigned int j = 0; j < extrudedTrianglesCount; ++j)
		{
			auto &extrudedTri = extrudedTris[j];
			auto &extrudedTriNormal = extrudedTrisNormals[j];

			if (extrudedTriNormal.Dot(dir) > 0.0f)
				continue;

			float magnitude = triNormal.Length();
			if (magnitude == 0.0f)
				continue;

			extrudedTriNormal /= magnitude;
			float intersectionDistance;
			vec3 intersectionPoint;
			vec3 intersectionNormal;
			if (!SweepSphereTriangle(center, dir * distance, radius, extrudedTri, intersectionDistance, intersectionPoint, intersectionNormal))
				continue;

			if (!foundCollision || intersectionDistance < nearestDistance)
			{
				foundCollision = true;
				nearestDistance = intersectionDistance;
				nearestTriangle = extrudedTri;
				nearestSrcTriangleIndex = i;
				nearestTriNormal = triNormal;
			}

		}

	}

	if (foundCollision)
	{
		result.nearestDistance = nearestDistance;
		result.collisionFound = true;

		vec3 newSphereCenter = center + dir*nearestDistance;

		float u, v;
		vec3 hit = MathUtils::ClosestPointTriangle(newSphereCenter, nearestTriangle.p0, nearestTriangle.p1, nearestTriangle.p2, u, v);

		result.nearestNormal = newSphereCenter - hit;
		if (result.nearestNormal.Length2() < 10e-5)
		{
			CalcNormal(nearestTriangle.p0, nearestTriangle.p1, nearestTriangle.p2, result.nearestNormal);
		}

		result.nearestNormal.Normalize();

		if (nearestSrcTriangleIndex < std::numeric_limits<unsigned int>::max())
		{
			result.nearestTriNormal = nearestTriNormal;

			auto &p0 = collisionMesh.triangles[nearestSrcTriangleIndex].p0;
			auto &p1 = collisionMesh.triangles[nearestSrcTriangleIndex].p1;
			auto &p2 = collisionMesh.triangles[nearestSrcTriangleIndex].p2;

			vec3 pointOnSeg, pointOnTri;
			pointOnTri = MathUtils::LineSegmentTriangleClosestPoint(capsuleP0 + dir * nearestDistance, capsuleP1 + dir * nearestDistance, p0, p1, p2, pointOnSeg);
			result.nearestIntersectionPoint = pointOnTri;
		}


	}
	return foundCollision;
}