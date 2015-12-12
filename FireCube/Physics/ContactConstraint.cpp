#include "ContactConstraint.h"

using namespace FireCube;

ContactConstraint::ContactConstraint(RigidBody *bodyA, RigidBody *bodyB, float maxForce) : Constraint(bodyA, bodyB, 0.0f, maxForce), restitution(0.0f), ri(0.0f), rj(0.0f), ni(0.0f)
{

}

float ContactConstraint::ComputeB(float a, float b, float h)
{
	vec3 vi = bi->GetVelocity();
	vec3 wi = bi->GetAngularVelocity();
	vec3 fi = bi->GetForce();
	
	vec3 vj = bj->GetVelocity();
	vec3 wj = bj->GetAngularVelocity();
	vec3 fj = bj->GetForce();
	
	vec3 penetrationVec;

	auto &GA = jacobianA;
	auto &GB = jacobianB;

	vec3 n = ni;

	// Calculate cross products
	vec3 rixn = Cross(ri, n);
	vec3 rjxn = Cross(rj, n);

	// g = xj+rj -(xi+ri)
	// G = [ -ni  -rixn  ni  rjxn ]
	GA.linear = -n;
	GA.rotational = -rixn;	
	GB.linear = n;
	GB.rotational = rjxn;

	// Calculate the penetration vector
	penetrationVec = bj->GetPosition() + rj - (bi->GetPosition() + ri);	

	float g = n.Dot(penetrationVec);
	
	float ePlusOne = restitution + 1.0f;
	float GW = ePlusOne * vj.Dot(n) - ePlusOne * vi.Dot(n) + wj.Dot(rjxn) - wi.Dot(rixn);
	float GiMf = ComputeGiMf();

	float B = -g * a - GW * b - h*GiMf;

	return B;
}

void ContactConstraint::SetRestitution(float restitution)
{
	this->restitution = restitution;
}