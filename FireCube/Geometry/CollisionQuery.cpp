#include "CollisionQuery.h"
using namespace FireCube;

RayQuery::RayQuery(const Ray &ray, float maxDistance) : ray(ray), maxDistance(maxDistance)
{

}

const std::vector<RayQueryResult> &RayQuery::GetResult() const
{
	return results;
}

EllipsoidQuery::EllipsoidQuery(vec3 position, vec3 velocity, vec3 radius) : position(position), velocity(velocity), 
	radius(radius), normalizedVelocity(velocity.Normalized())
{

}