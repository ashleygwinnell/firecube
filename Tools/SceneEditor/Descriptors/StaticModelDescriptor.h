#pragma once

#include "ComponentDescriptor.h"

class StaticModelDescriptor : public ComponentDescriptor
{
public:
	StaticModelDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~StaticModelDescriptor();

	void SetMeshFilename(const std::string &meshFilename, FireCube::Engine *engine);
	std::string GetMeshFilename() const;
	bool GetCastShadow() const;
	void SetCastShadow(bool castShadow);
	unsigned int GetLightMask() const;
	void SetLightMask(unsigned int lightMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
	const std::vector<std::string> &GetRenderablePartsMaterials() const;
	void SetRenderablePartMaterial(unsigned int index, const std::string &material, FireCube::Engine *engine = nullptr);
private:
	std::string meshFilename;
	bool castShadow;
	unsigned int lightMask;
	unsigned int collisionQueryMask;
	std::vector<std::string> renderablePartsMaterials;
};