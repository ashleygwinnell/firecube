#pragma once

#include "Renderable.h"
#include "Geometry/ModelLoader.h"

namespace FireCube
{

class StaticModel : public Renderable
{	
public:
	StaticModel(Engine *engine);
	void LoadMesh(const std::string &filename, ModelLoadingOptions options = ModelLoadingOptions());
	void SetBoundingBox(BoundingBox boundingBox);
protected:
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	std::vector<GeometryPtr> geometries;
};

}