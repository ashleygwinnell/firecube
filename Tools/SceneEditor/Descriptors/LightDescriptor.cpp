#include "FireCube.h"
#include "LightDescriptor.h"

using namespace FireCube;

LightDescriptor::LightDescriptor() : ComponentDescriptor(ComponentType::LIGHT), range(50.0f), spotCutOff(PI * 0.25f), castShadow(false), lightMask(0xFFFFFFFF), shadowIntensity(0.5f)
{

}

void LightDescriptor::CreateComponent(Node *node, Engine *engine)
{
	auto light = node->CreateComponent<Light>();
	light->SetLightType(lightType);
	light->SetCastShadow(castShadow);
	light->SetShadowIntensity(shadowIntensity);
	light->SetColor(color);
	light->SetRange(range);
	light->SetSpotCutOff(spotCutOff);
	light->SetLightMask(lightMask);
	
	component = light;
}

ComponentDescriptor *LightDescriptor::Clone()
{
	LightDescriptor *clone = new LightDescriptor();
	clone->lightType = lightType;
	clone->color = color;
	clone->castShadow = castShadow;
	clone->shadowIntensity = shadowIntensity;
	clone->range = range;
	clone->spotCutOff = spotCutOff;
	clone->lightMask = lightMask;
	return clone;
}

LightDescriptor::~LightDescriptor()
{

}

void LightDescriptor::SetLightType(FireCube::LightType lightType)
{
	this->lightType = lightType;
	if (component)
	{
		((Light *)component)->SetLightType(lightType);
	}
}
FireCube::LightType LightDescriptor::GetLightType() const
{
	return lightType;
}
void LightDescriptor::SetColor(FireCube::vec3 color)
{
	this->color = color;
	if (component)
	{
		((Light *)component)->SetColor(color);
	}
}

FireCube::vec3 LightDescriptor::GetColor() const
{
	return color;
}

bool LightDescriptor::GetCastShadow() const
{
	return castShadow;
}
void LightDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((Light *)component)->SetCastShadow(castShadow);
	}
}

void LightDescriptor::SetShadowIntensity(float shadowIntensity)
{
	this->shadowIntensity = shadowIntensity;
	if (component)
	{
		((Light *)component)->SetShadowIntensity(shadowIntensity);
	}
}

float LightDescriptor::GetShadowIntensity() const
{
	return shadowIntensity;
}

void LightDescriptor::SetRange(float range)
{
	this->range = range;
	if (component)
	{
		((Light *)component)->SetRange(range);
	}
}

float LightDescriptor::GetRange() const
{
	return range;
}

void LightDescriptor::SetSpotCutOff(float spotCutOff)
{
	this->spotCutOff = spotCutOff;
	if (component)
	{
		((Light *)component)->SetSpotCutOff(spotCutOff);
	}
}

float LightDescriptor::GetSpotCutOff() const
{
	return spotCutOff;
}

unsigned int LightDescriptor::GetLightMask() const
{
	return lightMask;
}

void LightDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((Light *)component)->SetLightMask(lightMask);
	}
}