#pragma once

#include "ComponentDescriptor.h"

class PlaneDescriptor : public ComponentDescriptor
{
public:
	PlaneDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~PlaneDescriptor();

	void SetSize(FireCube::vec2 size, FireCube::Engine *engine);
	FireCube::vec2 GetSize() const;
	void SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine);
	std::string GetMaterialFileName();
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
private:
	FireCube::vec2 size;
	std::string materialFileName;
	bool castShadow;
	unsigned int lightMask;
	unsigned int collisionQueryMask;
};