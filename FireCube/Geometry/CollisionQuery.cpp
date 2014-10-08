#include "CollisionQuery.h"
using namespace FireCube;

RayQuery::RayQuery(const Ray &ray, float maxDistance) : ray(ray), maxDistance(maxDistance)
{

}

const std::vector<RayQueryResult> &RayQuery::GetResult() const
{
	return results;
}