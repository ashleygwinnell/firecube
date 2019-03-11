#include "PhysicsCollisionQuery.h"

using namespace FireCube;

PhysicsRayQuery::PhysicsRayQuery(const Ray &ray, float maxDistance)
{
	this->ray = ray;
	this->maxDistance = maxDistance;
}

const std::vector<PhysicsRayQueryResult> &PhysicsRayQuery::GetResult() const
{
	return results;
}
