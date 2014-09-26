#include "Math/Plane.h"

using namespace FireCube;

Plane::Plane()
{

}

Plane::Plane(const vec3 &normal, float distance)
{
	this->normal = normal;
	this->distance = distance;
}

Plane::Plane(const vec3 &p1, const vec3 &p2, const vec3 &p3)
{
	normal.Cross(p2 - p1, p3 - p1);
	normal.Normalize();
	distance = p1.Dot(normal);
}

void Plane::SetNormal(const vec3 &normal)
{
	this->normal = normal;
}

vec3 Plane::GetNormal() const
{
	return normal;
}

void Plane::SetDistance(float distance)
{
	this->distance = distance;
}

float Plane::GetDistance() const
{
	return distance;
}

float Plane::GetDistance(const vec3 &point) const
{
	return normal.Dot(point) - distance;
}

bool Plane::IsFrontFacingTo(const vec3 &dir) const
{
	return (normal.Dot(dir) <= 0.0f);
}