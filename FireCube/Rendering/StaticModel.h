#pragma once

#include "Renderable.h"

namespace FireCube
{

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class StaticModel : public Renderable
{	
public:
	StaticModel(Engine *engine);	
	void CreateFromMesh(MeshPtr mesh);
	void SetBoundingBox(BoundingBox boundingBox);
	std::vector<GeometryPtr> &GetGeometries();
	std::vector<MaterialPtr> &GetMaterials();
	void AddRenderablePart(GeometryPtr geometry, MaterialPtr material);
	virtual void IntersectRay(RayQuery &rayQuery);
protected:
	virtual void UpdateWorldBoundingBox();
	BoundingBox boundingBox;
	std::vector<GeometryPtr> geometries;
	std::vector<MaterialPtr> materials;
};

}