#pragma once

#include "ComponentDescriptor.h"

class LightDescriptor : public ComponentDescriptor
{
public:
	LightDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~LightDescriptor();

	void SetLightType(FireCube::LightType lightType);
	FireCube::LightType GetLightType() const;
	void SetColor(FireCube::vec4 color);
	FireCube::vec4 GetColor() const;
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	void SetShadowIntensity(float shadowIntensity);
	float GetShadowIntensity() const;
	void SetRange(float range);
	float GetRange() const;
	void SetSpotCutOff(float spotCutoff);
	float GetSpotCutOff() const;
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	
private:
	FireCube::LightType lightType;
	FireCube::vec4 color;
	bool castShadow;
	float shadowIntensity;
	float range;
	float spotCutOff;
	unsigned int lightMask;	
};