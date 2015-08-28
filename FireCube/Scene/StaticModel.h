#pragma once

#include "Scene/Renderable.h"

namespace FireCube
{

class Mesh;
class SkeletonNode;

class StaticModel : public Renderable
{	
	OBJECT(StaticModel)
public:
	StaticModel(Engine *engine);
	StaticModel(Engine *engine, Mesh *mesh);
	void CreateFromMesh(Mesh *mesh);
	std::vector<SharedPtr<Geometry>> &GetGeometries();
	std::vector<SharedPtr<Material>> &GetMaterials();	
	Mesh *GetMesh();
	virtual void IntersectRay(RayQuery &rayQuery);
	virtual Component *Clone() const;
protected:

	StaticModel(const StaticModel &other);

	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	void CreateRenderableParts(SkeletonNode &skeletonNode, std::vector<RenderablePart> &renderableParts, mat4 transformation);
	virtual void UpdateRenderableParts();
	Mesh *mesh;
	BoundingBox boundingBox;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;

	std::vector<mat4> renderablePartsTransformations;
};

}