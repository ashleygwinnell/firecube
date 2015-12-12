#pragma once

#include "Physics/Constraint.h"

namespace FireCube
{

class ContactConstraint : public Constraint
{
public:
	ContactConstraint(RigidBody *bodyA, RigidBody *bodyB, float maxForce = 1e6f);
	virtual float ComputeB(float a, float b, float h);	
	void SetRestitution(float restitution);
public:
	float restitution;
	vec3 ri;
	vec3 rj;
	vec3 ni;
};

}