#include "FrictionConstraint.h"

using namespace FireCube;

FrictionConstraint::FrictionConstraint(RigidBody *bodyA, RigidBody *bodyB, float slipForce) : Constraint(bodyA, bodyB, -slipForce, slipForce), ri(0.0f), rj(0.0f), t(0.0f)
{

}

float FrictionConstraint::ComputeB(float a, float b, float h)
{
	// Calculate cross products
	vec3 rixt = Cross(ri, t);
	vec3 rjxt = Cross(rj, t);

	// G = [-t -rixt t rjxt]
	// And remember, this is a pure velocity constraint, g is always zero!
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	GA.linear = -t;
	GA.rotational = -rixt;
	GB.linear = t;
	GB.rotational = rjxt;
	
	float GW = ComputeGW();
	float GiMf = ComputeGiMf();

	float B = -GW * b - h * GiMf;

	return B;
}
