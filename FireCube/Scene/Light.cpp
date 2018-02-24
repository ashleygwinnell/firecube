#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Scene/Camera.h"

using namespace FireCube;

Light::Light(Engine *engine) : Component(engine), range(50.0f), spotCutOff(PI * 0.25f), castShadow(false), lightMask(0xFFFFFFFF), shadowIntensity(0.5f)
{
	cameraNode = new Node(engine);
	camera = cameraNode->CreateComponent<Camera>();
}

Light::Light(const Light &other) : Component(engine), type(other.type), color(other.color), range(other.range), spotCutOff(other.spotCutOff), castShadow(other.castShadow), lightMask(other.lightMask),
								   shadowIntensity(other.shadowIntensity)
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

void Light::SetColor(const vec3 &color)
{
	this->color = color;
}

vec3 Light::GetColor() const
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
	
}

void Light::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		oldScene->RemoveLight(this);
	}

	if (scene)
	{
		scene->AddLight(this);
	}	
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

void Light::SetShadowIntensity(float shadowIntensity)
{
	this->shadowIntensity = shadowIntensity;
}

float Light::GetShadowIntensity() const
{
	return shadowIntensity;
}

void Light::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	switch (type)
	{
	case FireCube::LightType::DIRECTIONAL:
	{
		mat3 rotationMat = node->GetWorldRotation().GetMatrix();
		vec3 right(rotationMat.m[0], rotationMat.m[1], rotationMat.m[2]);
		vec3 up(rotationMat.m[3], rotationMat.m[4], rotationMat.m[5]);
		vec3 dir(rotationMat.m[6], rotationMat.m[7], rotationMat.m[8]);
		vec3 origin = node->GetWorldPosition();
		const unsigned int countX = 5;
		const unsigned int countY = 5;
		const float spacing = 1.5f;
		const float length = 3;
		float width = (countX - 1) * spacing;
		float height = (countX - 1) * spacing;
		for (unsigned int y = 0; y < countY; ++y)
		{
			for (unsigned int x = 0; x < countX; ++x)
			{
				vec3 p0 = origin + (-width * 0.5f + x * spacing) * right + (-height * 0.5f + y * spacing) * up;
				vec3 p1 = p0 - dir * length;
				debugRenderer->AddLine(p0, p1, vec3(0, 1, 0));
				debugRenderer->AddLine(p1, p1 - right * spacing * 0.3f - up * spacing * 0.3f + dir * spacing * 0.7f, vec3(1, 0, 0));
				debugRenderer->AddLine(p1, p1 + right * spacing * 0.3f - up * spacing * 0.3f + dir * spacing * 0.7f, vec3(1, 0, 0));
				debugRenderer->AddLine(p1, p1 + right * spacing * 0.3f + up * spacing * 0.3f + dir * spacing * 0.7f, vec3(1, 0, 0));
				debugRenderer->AddLine(p1, p1 - right * spacing * 0.3f + up * spacing * 0.3f + dir * spacing * 0.7f, vec3(1, 0, 0));
			}
		}
	}
	break;
	case FireCube::LightType::POINT:
		debugRenderer->AddSphere(node->GetWorldPosition(), range, 16, 16, vec3(0, 1, 0));
		break;
	case FireCube::LightType::SPOT:
	{
		Frustum frustum;
		mat4 projection = mat4::IDENTITY;
		projection.GeneratePerspective(spotCutOff * 180.0f / PI, 1.0f, 0.1f, range);
		frustum.Extract(camera->GetViewMatrix(), projection);
		debugRenderer->AddFrustum(frustum, vec3(0, 1, 0));
		break;
	}
	default:
		break;
	}
}

Component *Light::Clone() const
{
	Light *clone = new Light(*this);
	return clone;
}