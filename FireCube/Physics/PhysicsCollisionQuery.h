#pragma once
#include "Math/Ray.h"

namespace FireCube
{
	class CollisionShape;

	/**
	* Stores information about a ray query result
	*/
	class FIRECUBE_API PhysicsRayQueryResult
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

		CollisionShape *shape;
	};

	class FIRECUBE_API PhysicsRayQuery
	{
	public:

		/**
		* Constructs the ray query
		* @param ray The query ray
		* @param maxDistance The maximum intersection distance to consider
		*/
		PhysicsRayQuery(const Ray &ray, float maxDistance);

		/**
		* Returns the results of the intersection query
		*/
		const std::vector<PhysicsRayQueryResult> &GetResult() const;

		/**
		* The query ray
		*/
		Ray ray;

		/**
		* The list of results
		*/
		std::vector<PhysicsRayQueryResult> results;

		/**
		* Maximum distance for intersections
		*/
		float maxDistance;
	};

}