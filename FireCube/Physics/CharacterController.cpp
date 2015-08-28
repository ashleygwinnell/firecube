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

CharacterController::CharacterController(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f), contactOffset(0.1f), radius(0.5f), height(1.0f), stepOffset(0.3f)
{

}

CharacterController::CharacterController(const CharacterController &other) : Component(other), physicsWorld(other.physicsWorld), velocity(other.velocity), radius(other.radius), height(other.height),
																			 contactOffset(other.contactOffset)
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

void CharacterController::SetRadius(float radius)
{
	this->radius = radius;
}

float CharacterController::GetRadius() const
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
	vec3 p0 = node->GetWorldPosition() - vec3(0, height * 0.5f, 0);
	vec3 p1 = node->GetWorldPosition() + vec3(0, height * 0.5f, 0);
	debugRenderer->AddCapsule(p0, p1, radius, 8, 16, vec3(0, 1, 0));	
	//debugRenderer->AddSphere(node->GetWorldPosition(), radius, 16, 16, vec3(0, 1, 0));

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

void CharacterController::SetHeight(float height)
{
	this->height = height;
}

float CharacterController::GetHeight() const
{
	return height;
}

void CharacterController::SetContactOffset(float contactOffset)
{
	this->contactOffset = contactOffset;
}

float CharacterController::GetContactOffset() const
{
	return contactOffset;
}

void CharacterController::SetStepOffset(float stepOffset)
{
	this->stepOffset = stepOffset;
}

float CharacterController::GetStepOffset() const
{
	return stepOffset;
}