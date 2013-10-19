#pragma once
#include "Utils/utils.h"
#include "Core/Resource.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

class Engine;
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;

class FIRECUBE_API Mesh : public Resource
{
public:
	Mesh(Engine *engine);
	virtual bool Load(const std::string &filename);
	const std::vector<GeometryPtr> &GetGeometries() const;
	const BoundingBox &GetBoundingBox() const;
private:
	BoundingBox boundingBox;
	std::vector<GeometryPtr> geometries;
};

typedef std::shared_ptr<Mesh> MeshPtr;

}