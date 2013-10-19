#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/Scene.h"

using namespace FireCube;

Light::Light(Engine *engine) : Component(engine), range(50.0f), spotCutOff(0.8f)
{

}

void Light::SetLightType(LightType type)
{
	this->type = type;
}

LightType Light::GetLightType() const
{
	return type;
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

void Light::SetRange(float range)
{
	this->range = range;
}

float Light::GetRange() const
{
	return range;
}

void Light::SetSpotCutOff(float cutOff)
{
	spotCutOff = cutOff;
}

float Light::GetSpotCutOff() const
{
	return spotCutOff;
}

bool Light::operator == (const Light &other) const
{
	return diffuseColor == other.diffuseColor && specularColor == other.specularColor && type == other.type;
}

void Light::NodeChanged()
{
	if (node && node->GetScene())
		node->GetScene()->AddLight(this);
}
