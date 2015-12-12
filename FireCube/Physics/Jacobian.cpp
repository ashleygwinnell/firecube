#include "Jacobian.h"

using namespace FireCube;

Jacobian::Jacobian()
{

}

float Jacobian::MultiplyVectors(const vec3 &linear, const vec3 &rotational) const
{
	return linear.Dot(this->linear) + rotational.Dot(this->rotational);
}

