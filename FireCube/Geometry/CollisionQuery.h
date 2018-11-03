#pragma once

#include "Utils/utils.h"
#include "Math/Ray.h"

namespace FireCube
{

class Renderable;
class RenderablePart;

/**
* Stores information about a ray query result
*/
class FIRECUBE_API RayQueryResult
{
public:

	/**
	* Distance along the of the intersection point
	*/
	float distance;
	
	/**
	* Normal at intersection
	*/
	vec3 normal;
	
	/**
	* Renderable associated with this intersection
	*/
	Renderable *renderable;

	/**
	* RenderablePart associated with this intersection
	*/
	RenderablePart *renderablePart;
};

class FIRECUBE_API RayQuery
{	
public:
	
	/**
	* Constructs the ray query
	* @param ray The query ray
	* @param maxDistance The maximum intersection distance to consider
	*/
	RayQuery(const Ray &ray, float maxDistance);
	
	/**
	* Retrusn the results of the intersection query
	*/
	const std::vector<RayQueryResult> &GetResult() const;

	/**
	* The query ray
	*/
	Ray ray;
	
	/**
	* The list of results
	*/
	std::vector<RayQueryResult> results;
	
	/**
	* Maximum distance for intersections
	*/
	float maxDistance;
};

}