#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Component.h"

namespace FireCube
{

class Scene;

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
	//OBJECT(Light)
public:
	Light(Engine *engine);
	/**
	* Sets the light type.
	* @param type The new light type.
	*/
	void SetType(LightType type);

	/**
	* Gets the type of this light.
	* @return The type of this light.
	*/
	LightType GetType() const;

	/**
	* Sets the diffuse color of this light.
	* @param color The diffuse color to set.
	*/
	void SetDiffuseColor(const vec4 &color);

	/**
	* Gets the diffuse color of this light.
	* @return The diffuse color of this light.
	*/
	vec4 GetDiffuseColor() const;

	/**
	* Sets the specular color of this light.
	* @param color The specular color to set.
	*/
	void SetSpecularColor(const vec4 &color);

	/**
	* Gets the specular color of this light.
	* @return The specular color of this light.
	*/
	vec4 GetSpecularColor() const;

	void SetRange(float range);

	float GetRange() const;
	
	void SetSpotCutOff(float cutOff);
	float GetSpotCutOff() const;
	void SetScene(Scene *scene);

	/**
	* Checks whether two lights are equal.
	* @return True if the two lights are equal, false otherwise.
	*/
	bool operator == (const Light &other) const;
protected:
	virtual void MarkedDirty() {}
	virtual void NodeChanged();
private:
	LightType type;	
	vec4 diffuseColor;
	vec4 specularColor;
	float range;
	float spotCutOff;
	Scene *scene;
};
}