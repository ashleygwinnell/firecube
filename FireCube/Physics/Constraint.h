#pragma once

#include "Physics/RigidBody.h"
#include "Physics/Jacobian.h"

namespace FireCube
{

class Constraint
{
public:
	Constraint(RigidBody *bi, RigidBody *bj, float minForce = -1e6f, float maxForce = 1e6f);	
	virtual float ComputeB(float a, float b, float h);
	float ComputeGq() const;
	float ComputeGW() const;
	float ComputeGWlambda() const;
	float ComputeGiMf() const;
	float ComputeGiMGt() const;
	void AddToWlambda(float deltalambda);
	float ComputeC(float eps) const;
public:
	RigidBody *bi;
	RigidBody *bj;
	float minForce;
	float maxForce;
	
	Jacobian jacobianA;
	Jacobian jacobianB;

	bool enabled;	
};

}