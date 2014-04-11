#pragma once

#include "Renderable.h"

namespace FireCube
{

class Mesh;

class StaticModel : public Renderable
{	
public:
	StaticModel(Engine *engine);	
	void CreateFromMesh(Mesh *mesh);
	void SetBoundingBox(BoundingBox boundingBox);
	std::vector<Geometry *> &GetGeometries();
	std::vector<Material *> &GetMaterials();
	void AddRenderablePart(Geometry *geometry, Material *material);
	virtual void IntersectRay(RayQuery &rayQuery);
protected:
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	std::vector<Geometry *> geometries;
	std::vector<Material *> materials;
};

}