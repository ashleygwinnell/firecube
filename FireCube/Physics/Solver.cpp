#include "Solver.h"
#include "Physics/PhysicsWorld.h"
#include "Constraint.h"

using namespace FireCube;

// Implementation based on cannon.js library (http://www.cannonjs.org/)
Solver::Solver() : maxIterations(10), tolerance(1e-7f)
{

}

void Solver::AddConstraint(Constraint *constraint)
{
	constraints.push_back(constraint);
}

void Solver::Clear()
{
	for (auto c : constraints)
	{
		delete c;
	}

	constraints.clear();
}

unsigned int Solver::Solve(float dt, PhysicsWorld *physicsWorld)
{
	unsigned int iter = 0;
	float tolSquared = tolerance * tolerance;		
	auto &bodies = physicsWorld->GetRigitBodies();		

	std::vector<float> invCs(constraints.size());
	std::vector<float> Bs(constraints.size());
	std::vector<float> lambda(constraints.size());

	for (unsigned int i = 0; i < constraints.size(); ++i)
	{
		auto c = constraints[i];
		lambda[i] = 0.0f;
		Bs[i] = c->ComputeB(a, b, dt);
		invCs[i] = 1.0f / c->ComputeC(eps);
	}

	if (constraints.empty() == false)
	{
		for (auto body : bodies)
		{
			body->wlambda.Set(0.0f, 0.0f, 0.0f);
			body->vlambda.Set(0.0f, 0.0f, 0.0f);
		}

		for (iter = 0; iter < maxIterations; ++iter)
		{
			float deltaLambdaTot = 0.0f;
			for (unsigned int j = 0; j < constraints.size(); ++j)
			{
				auto c = constraints[j];
				float B = Bs[j];
				float invC = invCs[j];
				float lambdaJ = lambda[j];
				float GWlambda = c->ComputeGWlambda();
				float deltaLambda = invC * (B - GWlambda - eps * lambdaJ);
				if (lambdaJ + deltaLambda < c->minForce) 
				{
					deltaLambda = c->minForce - lambdaJ;
				}
				else if (lambdaJ + deltaLambda > c->maxForce) 
				{
					deltaLambda = c->maxForce - lambdaJ;
				}

				lambda[j] += deltaLambda;

				deltaLambdaTot += std::abs(deltaLambda);

				c->AddToWlambda(deltaLambda);
			}

			if (deltaLambdaTot * deltaLambdaTot < tolSquared) 
			{
				break;
			}
		}

		for (auto body : bodies)
		{			
			body->SetVelocity(body->GetVelocity() + body->vlambda);
			body->SetAngularVelocity(body->GetAngularVelocity() + body->wlambda);
		}		
	}

	return iter;
}

void Solver::SetSpookParams(float stiffness, float relaxation, float timeStep)
{
	float d = relaxation;
	float k = stiffness;
	float h = timeStep;

	a = 4.0f / (h * (1.0f + 4.0f * d));
	b = (4.0f * d) / (1.0f + 4.0f * d);
	eps = 4.0f / (h * h * k * (1.0f + 4.0f * d));
}
