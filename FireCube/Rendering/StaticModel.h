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
	std::vector<Geometry *> &GetGeometries();
	std::vector<Material *> &GetMaterials();	
	virtual void IntersectRay(RayQuery &rayQuery);
protected:
	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	std::vector<Geometry *> geometries;
	std::vector<Material *> materials;
};

}