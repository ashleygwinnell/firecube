#pragma once

#include "ComponentDescriptor.h"

class TerrainDescriptor : public ComponentDescriptor
{
public:
	TerrainDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~TerrainDescriptor();

	void SetHeightmapFilename(const std::string &filename, FireCube::Engine *engine);
	std::string GetHeightmapFilename() const;
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
	void SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine);
	std::string GetMaterialFileName();
private:
	std::string heightmapFilename;
	bool castShadow;
	unsigned int lightMask;
	unsigned int collisionQueryMask;
	std::string materialFileName;
};