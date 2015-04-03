#include "Physics/RigidBody.h"
#include "Physics/PhysicsWorld.h"
#include "Scene/Node.h"
#include "Math/Plane.h"

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

void RigidBody::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

vec3 RigidBody::GetVelocity() const
{
	return velocity;
}