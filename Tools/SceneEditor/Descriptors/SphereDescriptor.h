#pragma once

#include "ComponentDescriptor.h"

class SphereDescriptor : public ComponentDescriptor
{
public:
	SphereDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~SphereDescriptor();

	void SetRadius(float radius, FireCube::Engine *engine);
	float GetRadius() const;
	void SetColumns(unsigned int columns, FireCube::Engine *engine);
	unsigned int GetColumns() const;
	void SetRings(unsigned int rings, FireCube::Engine *engine);
	unsigned int GetRings() const;
	void SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine);
	std::string GetMaterialFileName();
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
private:
	float radius;
	unsigned int columns;
	unsigned int rings;
	std::string materialFileName;
	bool castShadow;
	unsigned int lightMask;
	unsigned int collisionQueryMask;
};