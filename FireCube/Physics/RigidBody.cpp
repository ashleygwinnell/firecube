#include "Physics/RigidBody.h"
#include "Physics/PhysicsWorld.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Scene/Scene.h"

using namespace FireCube;

RigidBody::RigidBody(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f)
{

}

RigidBody::RigidBody(const RigidBody &other) : Component(engine), velocity(other.velocity), physicsWorld(other.physicsWorld)
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

Component *RigidBody::Clone() const
{
	RigidBody *clone = new RigidBody(*this);
	return clone;
}