#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/Viewport.h"

using namespace FireCube;

Light::Light(Engine *engine) : Component(engine)
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

bool Light::operator == (const Light &other) const
{
	return diffuseColor == other.diffuseColor && specularColor == other.specularColor && type == other.type;
}

void Light::NodeChanged()
{
	if (node && node->GetViewport())
		node->GetViewport()->AddLight(this);
}
