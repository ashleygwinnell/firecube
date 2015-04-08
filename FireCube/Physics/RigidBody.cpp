#include "Physics/RigidBody.h"
#include "Physics/PhysicsWorld.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Scene/Scene.h"

using namespace FireCube;

RigidBody::RigidBody(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f)
{

}

RigidBody::~RigidBody()
{
	if (physicsWorld)
	{
		physicsWorld->RemoveRigidBody(this);
	}
}

void RigidBody::MarkedDirty()
{
	
}

void RigidBody::NodeChanged()
{
	if (node)
	{
		physicsWorld = node->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddRigidBody(this);
	}
	else
	{

	}
}

void RigidBody::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		physicsWorld = oldScene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->RemoveRigidBody(this);
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddRigidBody(this);
	}
}

void RigidBody::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

vec3 RigidBody::GetVelocity() const
{
	return velocity;
}