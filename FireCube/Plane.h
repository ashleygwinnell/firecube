#ifndef PLANE_H
#define PLANE_H

#pragma warning(push)
#pragma warning(disable:4251)

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
	Plane();
	Plane(const vec3 &normal, float distance);
	void SetNormal(const vec3 &normal);
	vec3 GetNormal() const;
	void SetDistance(float distance);
	float GetDistance() const;
	float GetDistance(const vec3 &point) const;
private:
	vec3 normal;
	float distance;
};

}
#pragma warning(pop)
#endif