#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Math/MyMath.h"
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
	return normal.Dot(point) + distance;
}