#pragma once

#include "Math/Math.h"

namespace FireCube
{

class Jacobian
{
public:
	Jacobian();
	
	float MultiplyVectors(const vec3 &linear, const vec3 &rotational) const;
public:
	vec3 linear;
	vec3 rotational;
};

}