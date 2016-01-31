#pragma once

#include "ComponentDescriptor.h"

class RigidBodyDescriptor : public ComponentDescriptor
{
public:
	RigidBodyDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~RigidBodyDescriptor();

	void SetMass(float mass);
	float GetMass() const;
	
private:
	float mass;
};