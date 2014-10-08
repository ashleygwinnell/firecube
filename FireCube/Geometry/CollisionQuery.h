#pragma once

#include "Utils/utils.h"
#include "Math/Ray.h"

namespace FireCube
{

class Renderable;

class FIRECUBE_API RayQueryResult
{
public:
	float distance;
	vec3 normal;
	Renderable *renderable;
};

class FIRECUBE_API RayQuery
{	
public:
	RayQuery(const Ray &ray, float maxDistance);
	const std::vector<RayQueryResult> &GetResult() const;

	Ray ray;
	std::vector<RayQueryResult> results;
	float maxDistance;
};

}