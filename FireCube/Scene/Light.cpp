#include <string>
using namespace std;

#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Scene/Light.h"

using namespace FireCube;

void Light::SetType(LightType type)
{
	this->type = type;
}

LightType Light::GetType() const
{
	return type;
}

void Light::SetAmbientColor(const vec4 &color)
{
	ambientColor = color;
}

vec4 Light::GetAmbientColor() const
{
	return ambientColor;
}

void Light::SetDiffuseColor(const vec4 &color)
{
	diffuseColor = color;
}

vec4 Light::GetDiffuseColor() const
{
	return diffuseColor;
}

void Light::SetSpecularColor(const vec4 &color)
{
	specularColor = color;
}

vec4 Light::GetSpecularColor() const
{
	return specularColor;
}

bool Light::operator == (const Light &other) const
{
	return ambientColor == other.ambientColor && diffuseColor == other.diffuseColor && specularColor == other.specularColor && type == other.type;
}
