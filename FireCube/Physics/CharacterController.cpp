#include "Physics/CharacterController.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/CollisionShape.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Rendering/DebugRenderer.h"
#include "Math/MathUtils.h"
#include "Scene/Scene.h"
#include "Math/CollisionUtils.h"

using namespace FireCube;

CharacterController::CharacterController(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f)
{

}

CharacterController::CharacterController(const CharacterController &other) : Component(other), physicsWorld(other.physicsWorld), velocity(other.velocity), radius(other.radius)
{

}

CharacterController::~CharacterController()
{
	
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
		if (physicsWorld.Expired() == false)
		{
			physicsWorld->RemoveCharacterController(this);
		}		
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddCharacterController(this);
	}	
}

void CharacterController::CheckCollisionWithMesh(const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result) const
{	
	transform = scaleMatrix * transform;
	CollisionUtils::SweepEllipsoidMesh(transformedPosition, transformedVelocity, collisionMesh, transform, result);	
}

void CharacterController::CheckCollisionWithPlane(const Plane &plane, mat4 transform, CollisionResult &result) const
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
		result.contacts.push_back(CollisionContact());
		CollisionContact &e = result.contacts.back();
		e.time = t;
		e.distance = distToCollision;
		e.intersectionPoint = transformedPosition + t * transformedVelocity - n;
		e.normal = n;
		if ((result.collisionFound == false) || (distToCollision < result.nearestDistance))
		{			
			result.nearestTime = t;
			result.nearestDistance = distToCollision;
			result.nearestIntersectionPoint = e.intersectionPoint;
			result.nearestNormal = e.normal;
			result.collisionFound = true;
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
	contacts.clear();
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

	for (auto &c : contacts)
	{
		debugRenderer->AddLine(c.intersectionPoint * radius, (c.intersectionPoint + c.normal * 5.0f) * radius, vec3(0, 1, 0));
	}
}

bool CharacterController::IsOnGround() const
{
	return onGround;
}

Component *CharacterController::Clone() const
{
	CharacterController *clone = new CharacterController(*this);
	return clone;
}