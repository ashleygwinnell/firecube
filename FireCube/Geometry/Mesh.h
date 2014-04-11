#pragma once
#include "Utils/utils.h"
#include "Core/Resource.h"
#include "Math/BoundingBox.h"
#include "Geometry/Material.h"

namespace FireCube
{

class Engine;
class Geometry;

class FIRECUBE_API Mesh : public Resource
{
public:
	Mesh(Engine *engine);
	~Mesh();
	virtual bool Load(const std::string &filename);
	const std::vector<Geometry *> &GetGeometries() const;
	const std::vector<Material *> &GetMaterials() const;
	const BoundingBox &GetBoundingBox() const;
private:
	BoundingBox boundingBox;
	std::vector<Geometry *> geometries;
	std::vector<Material *> materials;
};

}