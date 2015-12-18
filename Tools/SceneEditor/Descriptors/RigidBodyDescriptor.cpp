#include "RigidBodyDescriptor.h"

using namespace FireCube;

#include "FireCube.h"
#include "BoxDescriptor.h"

using namespace FireCube;

RigidBodyDescriptor::RigidBodyDescriptor() : ComponentDescriptor(ComponentType::RIGID_BODY), mass(0.0f)
{

}

RigidBodyDescriptor::~RigidBodyDescriptor()
{

}

void RigidBodyDescriptor::CreateComponent(Node *node, Engine *engine)
{
	auto rigidBody = node->CreateComponent<RigidBody>();
	rigidBody->SetMass(mass);
	
	component = rigidBody;
}

ComponentDescriptor *RigidBodyDescriptor::Clone()
{
	RigidBodyDescriptor *clone = new RigidBodyDescriptor();
	clone->mass = mass;
	
	return clone;
}

void RigidBodyDescriptor::SetMass(float mass)
{
	this->mass = mass;
	if (component)
	{
		auto rigidBody = static_cast<RigidBody *>(component);
		rigidBody->SetMass(mass);
		
	}
}

float RigidBodyDescriptor::GetMass() const
{
	return mass;
}
