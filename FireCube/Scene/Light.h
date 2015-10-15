#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Component.h"

namespace FireCube
{

class Scene;
class Camera;

/**
* Specifies the type of a light.
*/
enum class LightType
{
	DIRECTIONAL, POINT, SPOT
};
/**
* A class storing various properties of a light source.
*/
class FIRECUBE_API Light : public Component
{
	OBJECT(Light)
public:
	Light(Engine *engine);
	/**
	* Sets the light type.
	* @param type The new light type.
	*/
	void SetLightType(LightType type);

	/**
	* Gets the type of this light.
	* @return The type of this light.
	*/
	LightType GetLightType() const;

	/**
	* Sets the color of this light.
	* @param color The color to set.
	*/
	void SetColor(const vec3 &color);

	/**
	* Gets the color of this light.
	* @return The color of this light.
	*/
	vec3 GetColor() const;
	
	void SetRange(float range);

	float GetRange() const;
	
	void SetSpotCutOff(float cutOff);
	float GetSpotCutOff() const;
	void SetScene(Scene *scene);

	Camera *GetCamera();

	/**
	* Checks whether two lights are equal.
	* @return True if the two lights are equal, false otherwise.
	*/
	bool operator == (const Light &other) const;

	void SetCastShadow(bool castShadow);
	bool GetCastShadow() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetLightMask() const;
	void SetShadowIntensity(float shadowIntensity);
	float GetShadowIntensity() const;
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	virtual Component *Clone() const;
protected:
	virtual void MarkedDirty() {}
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
private:

	Light(const Light &other);

	LightType type;	
	vec3 color;	
	float range;
	float spotCutOff;	
	SharedPtr<Node> cameraNode;
	Camera *camera;
	bool castShadow;
	unsigned int lightMask;
	float shadowIntensity;
};
}