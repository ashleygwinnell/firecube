#include "PhysicsWorldDescriptor.h"

using namespace FireCube;

PhysicsWorldDescriptor::PhysicsWorldDescriptor() : ComponentDescriptor(ComponentType::PHYSICS_WORLD)
{

}

void PhysicsWorldDescriptor::CreateComponent(FireCube::Node *node, FireCube::Engine *engine)
{
	auto physicsWorld = node->CreateComponent<PhysicsWorld>();
	component = physicsWorld;
}

ComponentDescriptor *PhysicsWorldDescriptor::Clone()
{
	return new PhysicsWorldDescriptor();
}

PhysicsWorldDescriptor::~PhysicsWorldDescriptor()
{

}