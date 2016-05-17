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
	FIRECUBE_OBJECT(Light)
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
	
	/**
	* Sets the range of this light
	* @param range The range to set
	*/
	void SetRange(float range);

	/**
	* @returns The range of this light
	*/
	float GetRange() const;
	
	/**
	* Sets the cut off angle for spot lights
	* @param cutOff The spot cut off angle in radians
	*/
	void SetSpotCutOff(float cutOff);
	
	/**
	* @returns The spot cut off angle
	*/	
	float GetSpotCutOff() const;
	
	
	void SetScene(Scene *scene);

	/**
	* @returns The camera of this light (used to render the scene using the light's point of view for shadow mapping)
	*/
	Camera *GetCamera();

	/**
	* Checks whether two lights are equal.
	* @return True if the two lights are equal, false otherwise.
	*/
	bool operator == (const Light &other) const;

	/**
	* Sets whether this light should cast shadows
	* @param castShadow Whether this light should cast shadows
	*/
	void SetCastShadow(bool castShadow);
	
	/**
	* @returns Whether this light casts shadows
	*/
	bool GetCastShadow() const;
	
	/**
	* Sets the light mask of this light. This controls which Renderables are affected by this light.
	* When a renderable's light mask and this light's light mask result in non-zero when AND together the rendeable is affected by this light
	* @param lightMask The light mask to set
	*/
	void SetLightMask(unsigned int lightMask);
	
	/**
	* @returns The light mask of this light
	*/
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