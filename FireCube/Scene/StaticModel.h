#pragma once

#include "Scene/Renderable.h"

namespace FireCube
{

class Mesh;
class SkeletonNode;

/**
* A class representing a renderable component of a static mesh.
*/
class StaticModel : public Renderable
{	
	FIRECUBE_OBJECT(StaticModel)
public:
	StaticModel(Engine *engine);
	StaticModel(Engine *engine, Mesh *mesh);
	
	/**
	* Creates the static model from a mesh resource
	* @mesh The mesh to use	
	*/
	void CreateFromMesh(Mesh *mesh);
	
	/**
	* @returns All the geometries in the model
	*/
	std::vector<SharedPtr<Geometry>> &GetGeometries();
	
	/**
	* Returns all the materials in this model. Note that the length of the returned array is the same as of the geometries.
	* The i'th material is used when rendering the i'th geometry. Therefore, the returned array might contain duplicates
	*/
	std::vector<SharedPtr<Material>> &GetMaterials();
				
	virtual void IntersectRay(RayQuery &rayQuery);
	
	/**
	* Clones this component
	*/
	virtual Component *Clone() const;
	
	/**
	* Sets a material to use when rendering all the geometries
	* @param material The material to set
	*/
	void SetMaterial(Material *material);

protected:

	StaticModel(const StaticModel &other);

	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	void CreateRenderableParts(Mesh *mesh, SkeletonNode &skeletonNode, std::vector<RenderablePart> &renderableParts, mat4 transformation);
	virtual void UpdateRenderableParts();
	void MeshReloaded(Resource *mesh);

	BoundingBox boundingBox;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;

	std::vector<mat4> renderablePartsTransformations;
};

}