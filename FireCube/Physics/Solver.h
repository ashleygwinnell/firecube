#pragma once

#include <vector>

namespace FireCube
{

class Constraint;
class PhysicsWorld;

class Solver
{
public:
	Solver();
	void AddConstraint(Constraint *constraint);
	void Clear();
	unsigned int Solve(float dt, PhysicsWorld *physicsWorld);
	void SetSpookParams(float stiffness, float relaxation, float timeStep);
private:
	std::vector<Constraint *> constraints;

	float a;
	float b;
	float eps;

	unsigned int maxIterations;
	float tolerance;
};

}