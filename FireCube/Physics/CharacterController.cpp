#include "Physics/CharacterController.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/CollisionShape.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Rendering/DebugRenderer.h"
#include "Math/MathUtils.h"
#include "Scene/Scene.h"

using namespace FireCube;

CharacterController::CharacterController(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f)
{

}

CharacterController::~CharacterController()
{
	if (physicsWorld)
	{
		physicsWorld->RemoveCharacterController(this);
	}
}

void CharacterController::MarkedDirty()
{

}
void CharacterController::NodeChanged()
{
	
}

void CharacterController::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		physicsWorld = oldScene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->RemoveCharacterController(this);
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddCharacterController(this);
	}	
}

void CharacterController::CheckCollisionWithMesh(const CollisionMesh &collisionMesh, mat4 transform)
{	
	transform = scaleMatrix * transform;
	for (const auto &tri : collisionMesh.triangles)
	{				
		const vec3 p1 = transform * tri.p1;
		const vec3 p2 = transform * tri.p2;
		const vec3 p3 = transform * tri.p3;
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
				collisions.push_back(CollisionEntry());
				CollisionEntry &e = collisions.back();
				e.time = t;
				e.distance = distToCollision;
				e.intersectionPoint = collisionPoint;
				e.normal = nearestNormal;
				if ((collisionFound == false) || (distToCollision < nearestDistance))
				{					
					this->nearestTime = t;
					this->nearestDistance = distToCollision;
					this->nearestIntersectionPoint = collisionPoint;
					this->nearestNormal = nearestNormal;
					this->collisionFound = true;				
				}
			}
		}
	}
}

void CharacterController::CheckCollisionWithPlane(const Plane &plane, mat4 transform)
{	
	vec4 p(plane.GetNormal(), -plane.GetDistance());
	transform = scaleMatrix * transform;
	transform.Inverse();
	transform.Transpose();
	p = transform * p;
	vec3 n = vec3(p.x, p.y, p.z);
	float l = n.Length();
	n = n / l;
	float d = -p.w / l;		
	float t = (1 + d - n.Dot(transformedPosition)) / n.Dot(transformedVelocity);
	if (t >= 0.0f && t <= 1.0f)
	{
		float distToCollision = t * transformedVelocity.Length();
		collisions.push_back(CollisionEntry());
		CollisionEntry &e = collisions.back();
		e.time = t;
		e.distance = distToCollision;
		e.intersectionPoint = transformedPosition + t * transformedVelocity - n;
		e.normal = n;
		if ((collisionFound == false) || (distToCollision < nearestDistance))
		{			
			this->nearestTime = t;
			this->nearestDistance = distToCollision;
			this->nearestIntersectionPoint = e.intersectionPoint;
			this->nearestNormal = e.normal;
			this->collisionFound = true;		
		}
	}
}

void CharacterController::UpdateTransformedState()
{
	scaleMatrix = mat4::IDENTITY;
	scaleMatrix.Scale(1.0f / radius.x, 1.0f / radius.y, 1.0f / radius.z);

	inverseTransposedTransform = scaleMatrix;	
	inverseTransposedTransform.Inverse();
	inverseTransposedTransform.Transpose();

	transformedPosition = node->GetWorldPosition() / radius;
	transformedVelocity = velocity / radius;
	normalizedTransformedVelocity = transformedVelocity.Normalized();
	collisions.clear();
	finishedMovement = false;
}

void CharacterController::UpdateFromTransformedState()
{
	node->SetTranslation(transformedPosition * radius);	
	velocity = transformedVelocity * radius;	
}

void CharacterController::SetRadius(vec3 radius)
{
	this->radius = radius;
}

vec3 CharacterController::GetRadius() const
{
	return radius;
}

void CharacterController::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

vec3 CharacterController::GetVelocity() const
{
	return velocity;
}

void CharacterController::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	vec3 p[8];
	p[0] = node->GetWorldTransformation() * vec3(-radius.x, -radius.y, radius.z);
	p[1] = node->GetWorldTransformation() * vec3(radius.x, -radius.y, radius.z);
	p[2] = node->GetWorldTransformation() * vec3(radius.x, radius.y, radius.z);
	p[3] = node->GetWorldTransformation() * vec3(-radius.x, radius.y, radius.z);
	p[4] = node->GetWorldTransformation() * vec3(-radius.x, -radius.y, -radius.z);
	p[5] = node->GetWorldTransformation() * vec3(-radius.x, radius.y, -radius.z);
	p[6] = node->GetWorldTransformation() * vec3(radius.x, radius.y, -radius.z);
	p[7] = node->GetWorldTransformation() * vec3(radius.x, -radius.y, -radius.z);
	debugRenderer->AddLine(p[0], p[1], vec3(0, 1, 0));
	debugRenderer->AddLine(p[1], p[2], vec3(0, 1, 0));
	debugRenderer->AddLine(p[2], p[3], vec3(0, 1, 0));
	debugRenderer->AddLine(p[3], p[0], vec3(0, 1, 0));
	debugRenderer->AddLine(p[4], p[5], vec3(0, 1, 0));
	debugRenderer->AddLine(p[5], p[6], vec3(0, 1, 0));
	debugRenderer->AddLine(p[6], p[7], vec3(0, 1, 0));
	debugRenderer->AddLine(p[7], p[4], vec3(0, 1, 0));
	debugRenderer->AddLine(p[0], p[4], vec3(0, 1, 0));
	debugRenderer->AddLine(p[3], p[5], vec3(0, 1, 0));
	debugRenderer->AddLine(p[1], p[7], vec3(0, 1, 0));
	debugRenderer->AddLine(p[2], p[6], vec3(0, 1, 0));

	for (auto &c : collisions)
	{
		debugRenderer->AddLine(c.intersectionPoint * radius, (c.intersectionPoint + c.normal * 5.0f) * radius, vec3(0, 1, 0));
	}
}

bool CharacterController::IsOnGround() const
{
	return onGround;
}