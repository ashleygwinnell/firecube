#include "Jacobian.h"

using namespace FireCube;

Jacobian::Jacobian() : linear(0.0f), rotational(0.0f)
{

}

float Jacobian::MultiplyVectors(const vec3 &linear, const vec3 &rotational) const
{
	return linear.Dot(this->linear) + rotational.Dot(this->rotational);
}

