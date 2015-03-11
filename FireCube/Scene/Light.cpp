#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/Scene.h"
#include "Scene/Camera.h"

using namespace FireCube;

Light::Light(Engine *engine) : Component(engine), range(50.0f), spotCutOff(PI * 0.25f), castShadow(false), lightMask(0xFFFFFFFF)
{
	cameraNode = new Node(engine);
	camera = cameraNode->CreateComponent<Camera>();
}

void Light::SetLightType(LightType type)
{
	this->type = type;
}

LightType Light::GetLightType() const
{
	return type;
}

void Light::SetColor(const vec4 &color)
{
	this->color = color;
}

vec4 Light::GetColor() const
{
	return color;
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
	return color == other.color && type == other.type;
}

void Light::NodeChanged()
{
	if (node && node->GetScene())
	{
		node->GetScene()->AddLight(this);
		scene = node->GetScene();
	}
	else if (!node && scene)
	{
		scene->RemoveLight(this);
	}
}

void Light::SetScene(Scene *scene)
{
	this->scene = scene;
}

Camera *Light::GetCamera()
{
	return camera;
}

void Light::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
}
bool Light::GetCastShadow() const
{
	return castShadow;
}

void Light::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
}

unsigned int Light::GetLightMask() const
{
	return lightMask;
}