#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include "Utils/utils.h"
#include "Math/Math.h"

namespace FireCube
{

// Forward declarations.
class vec3;

/**
* A class representing a plane.
*/
class FIRECUBE_API Plane
{
public:
	
	/**
	* Constructs a place.
	*/
	Plane();

	/**
	* Constructs a place.
	* @param normal The normal vector of the plane.
	* @param distance The distance from the origin.
	*/
	Plane(const vec3 &normal, float distance);

	/**
	* Sets the normal vector of this plane.
	* @param normal The new normal vector.
	*/
	void SetNormal(const vec3 &normal);

	/**
	* Returns the normal vector of this plane.
	*/
	vec3 GetNormal() const;
	
	/**
	* Sets the distance from the origin of this plane.
	* @param distance The new distance.
	*/
	void SetDistance(float distance);

	/**
	* Returns the distance from the origin of this plane.
	*/
	float GetDistance() const;

	/**
	* Returns the distance between a given point and this plane.
	* @param point The point to calculate the distance from.
	*/
	float GetDistance(const vec3 &point) const;

private:
	vec3 normal;
	float distance;
};

}
#pragma warning(pop)
