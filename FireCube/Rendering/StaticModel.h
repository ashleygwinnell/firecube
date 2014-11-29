#pragma once

#include "Renderable.h"

namespace FireCube
{

class Mesh;

class StaticModel : public Renderable
{	
	OBJECT(StaticModel)
public:
	StaticModel(Engine *engine);
	StaticModel(Engine *engine, Mesh *mesh);
	void CreateFromMesh(Mesh *mesh);	
	std::vector<SharedPtr<Geometry>> &GetGeometries();
	std::vector<SharedPtr<Material>> &GetMaterials();	
	virtual void IntersectRay(RayQuery &rayQuery);
protected:
	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;
};

}