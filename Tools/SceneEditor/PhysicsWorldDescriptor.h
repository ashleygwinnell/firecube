#pragma once

#include "ComponentDescriptor.h"

class PhysicsWorldDescriptor : public ComponentDescriptor
{
public:
	PhysicsWorldDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~PhysicsWorldDescriptor();
	
private:

};