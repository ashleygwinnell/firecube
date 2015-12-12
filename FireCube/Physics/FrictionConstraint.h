#pragma once

#include "Constraint.h"

namespace FireCube
{

class FrictionConstraint : public Constraint
{
public:
	FrictionConstraint(RigidBody *bodyA, RigidBody *bodyB, float slipForce);
	virtual float ComputeB(float a, float b, float h);
public:
	vec3 ri;
	vec3 rj;
	vec3 t;
};

}