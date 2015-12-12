#include "Constraint.h"

using namespace FireCube;

Constraint::Constraint(RigidBody *bi, RigidBody *bj, float minForce, float maxForce) : minForce(minForce), maxForce(maxForce), bi(bi), bj(bj), enabled(true)
{
	
}

float Constraint::ComputeB(float a, float b, float h)
{
	float GW = ComputeGW();
	float Gq = ComputeGq();
	float GiMf = ComputeGiMf();
	return -Gq * a - GW * b - GiMf * h;
}

float Constraint::ComputeGq() const
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	vec3 xi = bi->GetPosition();
	vec3 xj = bj->GetPosition();
	return GA.linear.Dot(xi) + GB.linear.Dot(xj);
}

float Constraint::ComputeGW() const
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	vec3 vi = bi->GetVelocity();
	vec3 vj = bj->GetVelocity();
	vec3 wi = bi->GetAngularVelocity();
	vec3 wj = bj->GetAngularVelocity();

	return GA.MultiplyVectors(vi, wi) + GB.MultiplyVectors(vj, wj);
}

float Constraint::ComputeGWlambda() const
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	
	auto &vi = bi->vlambda;
	auto &vj = bj->vlambda;
	auto &wi = bi->wlambda;
	auto &wj = bj->wlambda;

	return GA.MultiplyVectors(vi, wi) + GB.MultiplyVectors(vj, wj);
}

float Constraint::ComputeGiMf() const
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	vec3 fi = bi->GetForce();
	vec3 ti = bi->GetTorque();
	vec3 fj = bj->GetForce();
	vec3 tj = bj->GetTorque();
	float invMassi = bi->GetInvMass();
	float invMassj = bj->GetInvMass();

	vec3 iMfi = fi * invMassi;
	vec3 iMfj = fj * invMassj;
	
	return GA.MultiplyVectors(iMfi, bi->GetInvInertiaWorld() * ti) + GB.MultiplyVectors(iMfj, bj->GetInvInertiaWorld() * tj);
}

float Constraint::ComputeGiMGt() const
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	
	float invMassi = bi->GetInvMass();
	float invMassj = bj->GetInvMass();
	mat3 invIi = bi->GetInvInertiaWorld();
	mat3 invIj = bj->GetInvInertiaWorld();
	float result = invMassi + invMassj;

	vec3 tmp = invIi * GA.rotational;	
	result += tmp.Dot(GA.rotational);

	tmp = invIj * GB.rotational;	
	result += tmp.Dot(GB.rotational);

	return  result;
}

void Constraint::AddToWlambda(float deltaLambda)
{
	auto &GA = jacobianA;
	auto &GB = jacobianB;
	
	// Add to linear velocity
	// v_lambda += inv(M) * delta_lamba * G
	bi->vlambda += bi->GetInvMass() * deltaLambda * GA.linear;
	bj->vlambda += bj->GetInvMass() * deltaLambda * GB.linear;

	// Add to angular velocity	
	bi->wlambda += deltaLambda * bi->GetInvInertiaWorld() * GA.rotational;
	bj->wlambda += deltaLambda * bj->GetInvInertiaWorld() * GB.rotational;
}

float Constraint::ComputeC(float eps) const
{
	return ComputeGiMGt() + eps;
}
