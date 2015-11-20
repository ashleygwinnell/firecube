#include "CollisionUtils.h"
#include "MathUtils.h"
#include "Physics/CollisionShape.h"
#include "Physics/PhysicsWorld.h"
#include "Math/Plane.h"
#include "Math/Ray.h"

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
				if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
				{					
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
			if (fabs(signedDistToTrianglePlane) >= radius)
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
			vec3 planeIntersectionPoint = (position - trianglePlane.GetNormal() * radius) + velocity*t0;
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

void CollisionUtils::SweepSphereMesh(vec3 position, vec3 velocity, float radius, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result)
{
	result.collisionFound = false;
	for (const auto &tri : collisionMesh.triangles)
	{
		const vec3 &p1 = transform * tri.p0;
		const vec3 &p2 = transform * tri.p1;
		const vec3 &p3 = transform * tri.p2;
		const Plane &trianglePlane = Plane(p1, p2, p3);

		if (trianglePlane.IsFrontFacingTo(velocity))
		{
			float t0, t1;
			bool embeddedInPlane = false;

			float signedDistToTrianglePlane = trianglePlane.GetDistance(position);

			float normalDotVelocity = trianglePlane.GetNormal().Dot(velocity);

			if (normalDotVelocity == 0)
			{
				if (fabs(signedDistToTrianglePlane) >= radius)
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
				vec3 planeIntersectionPoint = (position - trianglePlane.GetNormal() * radius) + velocity*t0;
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
				if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
				{
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

bool CollisionUtils::SweepCapsuleMesh(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result)
{
	result.collisionFound = false;
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
		SweepSphereMesh(sphereCenter, dir * distance, radius, collisionMesh, transform, result);		
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
		
		vec3 originalTriNormal;
		CalcNormal(p0, p1, p2, originalTriNormal);
		originalTriNormal.Normalize();

		vec3 triNormal;
		p0 = transform * p0;
		p1 = transform * p1;
		p2 = transform * p2;
		CalcNormal(p0, p1, p2, triNormal);

		if (triNormal.Dot(dir) > 0.0f)
			continue;


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
				nearestTriNormal = originalTriNormal;
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

bool CollisionUtils::SweepCapsulePlane(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const Plane &plane, mat4 transform, CollisionResult &result)
{
	vec4 p(plane.GetNormal(), -plane.GetDistance());
	transform.Inverse();
	transform.Transpose();
	p = transform * p;
	vec3 n = vec3(p.x, p.y, p.z);
	float l = n.Length();
	n = n / l;
	float d = -p.w / l;
	
	unsigned int index = 0;
	vec3 pts[2];

	float minDp = 10e6;
	
	pts[0] = capsuleP0;
	pts[1] = capsuleP1;
	for (unsigned int i = 0; i < 2; i++)
	{
		const float dp = pts[i].Dot(n);
		if (dp < minDp)
		{
			minDp = dp;
			index = i;
		}
	}

	// test if the capsule initially overlaps with plane
	if (minDp <= radius + d)
	{
		result.nearestDistance = 0.0f;
		result.nearestNormal = -dir;
		result.collisionFound = true;
		return true;
	}
	
	const vec3 ptOnCapsule = pts[index] - n * radius;	
	bool hitPlane = Ray(ptOnCapsule, dir).IntersectPlane(Plane(n, d), result.nearestDistance);
	if (hitPlane && result.nearestDistance > 0 && result.nearestDistance <= distance)
	{
		result.nearestNormal = n;
		result.collisionFound = true;
		return true;
	}
	else
	{
		result.collisionFound = false;
		return false;
	}
	
}

bool CollisionUtils::SweepSpherePlane(vec3 position, vec3 velocity, float radius, const Plane &plane, mat4 transform, CollisionResult &result)
{
	vec4 p(plane.GetNormal(), -plane.GetDistance());
	transform.Inverse();
	transform.Transpose();
	p = transform * p;
	vec3 n = vec3(p.x, p.y, p.z);
	float l = n.Length();
	n = n / l;
	float d = -p.w / l;
	float t = (radius + d - n.Dot(position)) / n.Dot(velocity);
	if (t >= 0.0f && t <= 1.0f)
	{
		float distToCollision = t * velocity.Length();
		if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
		{
			result.nearestDistance = distToCollision;
			result.nearestIntersectionPoint = position + t * velocity - n;
			result.nearestNormal = n;
			result.collisionFound = true;
		}
	}

	return result.collisionFound;
}

int CollisionUtils::IntersectRayCapsule(vec3 origin, vec3 dir, vec3 capsuleP0, vec3 capsuleP1, float radius, float t[2])
{
	vec3 capsDir = capsuleP1 - capsuleP0;

	float fWLength = capsDir.Length();
	vec3 kW = capsDir / fWLength;

	if (fWLength < 0.0001f)
	{
		const float d0 = (origin - capsuleP0).Length2();
		const float d1 = (origin - capsuleP1).Length2();
		const float approxLength = (std::max(d0, d1) + radius) * 2.0f;
		return CollisionUtils::IntersectRaySphere(origin, dir, approxLength, capsuleP0, radius, t[0]) ? 1 : 0;
	}

	vec3 kU(0.0f);

	if (fWLength > 0.0f)
	{
		float fInvLength;
		if (std::abs(kW.x) >= std::abs(kW.y))
		{
			// W.x or W.z is the largest magnitude component, swap them
			fInvLength = 1.0f / std::sqrt(kW.x * kW.x + kW.z * kW.z);
			kU.x = -kW.z * fInvLength;
			kU.y = 0.0f;
			kU.z = kW.x * fInvLength;
		}
		else
		{
			// W.y or W.z is the largest magnitude component, swap them
			fInvLength = 1.0f / std::sqrt(kW.y * kW.y + kW.z * kW.z);
			kU.x = 0.0f;
			kU.y = kW.z * fInvLength;
			kU.z = -kW.y * fInvLength;
		}
	}

	vec3 kV = Cross(kW, kU);
	kV.Normalize();

	// compute intersection

	vec3 kD(kU.Dot(dir), kV.Dot(dir), kW.Dot(dir));
	const float fDLength = kD.Length();
	kD /= fDLength;

	const float fInvDLength = 1.0f / fDLength;
	const vec3 kDiff = origin - capsuleP0;
	const vec3 kP(kU.Dot(kDiff), kV.Dot(kDiff), kW.Dot(kDiff));
	const float fRadiusSqr = radius * radius;

	// Is the velocity parallel to the capsule direction? (or zero)
	if (std::abs(kD.z) >= 1.0f - 0.0001f || fDLength < 0.0001f)
	{
		const float fAxisDir = dir.Dot(capsDir);

		const float fDiscr = fRadiusSqr - kP.x*kP.x - kP.y*kP.y;
		if (fAxisDir < 0 && fDiscr >= 0.0f)
		{
			// Velocity anti-parallel to the capsule direction
			const float fRoot = std::sqrt(fDiscr);
			t[0] = (kP.z + fRoot)*fInvDLength;
			t[1] = -(fWLength - kP.z + fRoot)*fInvDLength;
			return 2;
		}
		else if (fAxisDir > 0 && fDiscr >= 0.0f)
		{
			// Velocity parallel to the capsule direction
			const float fRoot = std::sqrt(fDiscr);
			t[0] = -(kP.z + fRoot)*fInvDLength;
			t[1] = (fWLength - kP.z + fRoot)*fInvDLength;
			return 2;
		}
		else
		{
			// sphere heading wrong direction, or no velocity at all
			return 0;
		}
	}

	// test intersection with infinite cylinder
	float fA = kD.x*kD.x + kD.y*kD.y;
	float fB = kP.x*kD.x + kP.y*kD.y;
	float fC = kP.x*kP.x + kP.y*kP.y - fRadiusSqr;
	float fDiscr = fB*fB - fA*fC;
	if (fDiscr < 0.0f)
	{
		// line does not intersect infinite cylinder
		return 0;
	}

	int iQuantity = 0;

	if (fDiscr > 0.0f)
	{
		// line intersects infinite cylinder in two places
		const float fRoot = std::sqrt(fDiscr);
		const float fInv = 1.0f / fA;
		float fT = (-fB - fRoot)*fInv;
		float fTmp = kP.z + fT*kD.z;
		if (0.0f <= fTmp && fTmp <= fWLength)
			t[iQuantity++] = fT*fInvDLength;

		fT = (-fB + fRoot)*fInv;
		fTmp = kP.z + fT*kD.z;
		if (0.0f <= fTmp && fTmp <= fWLength)
			t[iQuantity++] = fT*fInvDLength;

		if (iQuantity == 2)
		{
			// line intersects capsule wall in two places
			return 2;
		}
	}
	else
	{
		// line is tangent to infinite cylinder
		const float fT = -fB / fA;
		const float fTmp = kP.z + fT*kD.z;
		if (0.0f <= fTmp && fTmp <= fWLength)
		{
			t[0] = fT*fInvDLength;
			return 1;
		}
	}

	// test intersection with bottom hemisphere
	// fA = 1
	fB += kP.z*kD.z;
	fC += kP.z*kP.z;
	fDiscr = fB*fB - fC;
	if (fDiscr > 0.0f)
	{
		const float fRoot = std::sqrt(fDiscr);
		float fT = -fB - fRoot;
		float fTmp = kP.z + fT*kD.z;
		if (fTmp <= 0.0f)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}

		fT = -fB + fRoot;
		fTmp = kP.z + fT*kD.z;
		if (fTmp <= 0.0f)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}
	else if (fDiscr == 0.0f)
	{
		const float fT = -fB;
		const float fTmp = kP.z + fT*kD.z;
		if (fTmp <= 0.0f)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}

	// test intersection with top hemisphere
	// fA = 1
	fB -= kD.z*fWLength;
	fC += fWLength*(fWLength - 2.0f*kP.z);

	fDiscr = fB*fB - fC;
	if (fDiscr > 0.0f)
	{
		const float fRoot = std::sqrt(fDiscr);
		float fT = -fB - fRoot;
		float fTmp = kP.z + fT*kD.z;
		if (fTmp >= fWLength)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}

		fT = -fB + fRoot;
		fTmp = kP.z + fT*kD.z;
		if (fTmp >= fWLength)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}
	else if (fDiscr == 0.0f)
	{
		const float fT = -fB;
		const float fTmp = kP.z + fT*kD.z;
		if (fTmp >= fWLength)
		{
			t[iQuantity++] = fT*fInvDLength;
			if (iQuantity == 2)
				return 2;
		}
	}

	return iQuantity;
}

bool CollisionUtils::IntersectRaySphere(const vec3 &origin, const vec3 &dir, float length, const vec3& center, float radius, float& dist)
{
	// get the offset vector
	const vec3 offset = center - origin;

	// get the distance along the ray to the center point of the sphere
	const float ray_dist = dir.Dot(offset);

	// get the squared distances
	const float off2 = offset.Dot(offset);
	const float rad_2 = radius * radius;
	if (off2 <= rad_2)
	{
		// we're in the sphere		
		dist = 0.0f;
		return true;
	}

	if (ray_dist <= 0 || (ray_dist - length) > radius)
	{
		// moving away from object or too far away
		return false;
	}

	// find hit distance squared
	const float d = rad_2 - (off2 - ray_dist * ray_dist);
	if (d < 0.0f)
	{
		// ray passes by sphere without hitting
		return false;
	}

	// get the distance along the ray
	dist = ray_dist - std::sqrt(d);
	if (dist > length)
	{
		// hit point beyond length
		return false;
	}
	
	return true;
}

bool CollisionUtils::SweepSphereCapsule(const vec3& sphereCenter, float sphereRadius, const vec3& capusleP0, const vec3& capusleP1, float radius, const vec3& dir, float length, CollisionResult &result)
{
	const float radiusSum = radius + sphereRadius;

	// Raycast against it
	float s[2];
	int n = CollisionUtils::IntersectRayCapsule(sphereCenter, dir, capusleP0, capusleP1, radiusSum, s);
	if (n)
	{
		float t;
		if (n == 1)
			t = s[0];
		else
			t = (s[0] < s[1]) ? s[0] : s[1];

		if (t >= 0.0f && t <= length)
		{
			result.nearestDistance = t;

			// Move capsule against sphere
			const vec3 tdir = t*dir;
			vec3 p0 = capusleP0 - tdir;
			vec3 p1 = capusleP1 - tdir;

			// Compute closest point between moved capsule & sphere
			MathUtils::DistancePointSegmentSquared(p0, p1, sphereCenter, &t);
			result.nearestIntersectionPoint = (1 - t) * p0 + t * p1;

			// Normal
			result.nearestNormal = (result.nearestIntersectionPoint - sphereCenter).Normalized();

			result.nearestIntersectionPoint -= result.nearestNormal * radius;
			result.collisionFound = true;
			return true;
		}
	}

	result.collisionFound = false;
	return false;
}