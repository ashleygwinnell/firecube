#pragma once

#include "ComponentDescriptor.h"

class GridDescriptor : public ComponentDescriptor
{
public:
	GridDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~GridDescriptor();

	void SetSize(FireCube::vec2 size, FireCube::Engine *engine);
	void SetCountX(unsigned int count, FireCube::Engine *engine);
	void SetCountZ(unsigned int count, FireCube::Engine *engine);
	FireCube::vec2 GetSize() const;
	unsigned int GetCountX() const;
	unsigned int GetCountZ() const;
	void SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine);
	std::string GetMaterialFileName();
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
private:
	void Recreate(FireCube::Engine *engine);

	FireCube::vec2 size;
	unsigned int countX, countZ;
	std::string materialFileName;
	bool castShadow;
	unsigned int lightMask;
	unsigned int collisionQueryMask;
};