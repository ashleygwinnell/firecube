#pragma once

#include <vector>
#include "Utils/utils.h"
#include "Core/Component.h"
#include "Rendering/RenderQueue.h"
#include "Math/BoundingBox.h"
#include "Math/Ray.h"
#include "Geometry/CollisionQuery.h"

namespace FireCube
{

template <class T>
class OctreeNode;
class Geometry;
class Scene;
class DebugRenderer;

/**
* A class representing a single renderable part which consists of at least a geometry, a material and transformation

*/
class RenderablePart
{
public:
	/**
	* The geometry of this part.
	*/
	Geometry *geometry;

	/**
	* The material of this part
	*/
	Material *material;

	/**
	* The transformation to apply to this part. This transformation should transform the geometry to world space
	*/
	mat4 transformation;

	/**
	* An array of skinning matrices
	*/
	mat4 *skinMatrices;

	/**
	* Number of skinned matrices
	*/
	unsigned int skinMatricesCount;
};

/**
* This class is a base class for all renderable components. Child components should
* fill the renderableParts member with appropriate renderable parts
*/
class FIRECUBE_API Renderable : public Component
{	
	FIRECUBE_OBJECT(Renderable)
	friend class Scene;
public:
	Renderable(Engine *engine);
	virtual ~Renderable();
	
	/**
	* @returns All the renderable parts in this Renderable
	*/
	const std::vector<RenderablePart> &GetRenderableParts() const;
	
	/**
	* @returns The world bounding box of this renderable
	*/
	BoundingBox GetWorldBoundingBox();	
	
	/**
	* Sets the collision query mask of this Renderable.
	* When a ray query collision query mask and this renderable's collision query mask result in non-zero when AND together, no intersection test is performed
	* @param lightMask The light mask to set
	*/
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
	
	/**
	* @returns The collision query mask of this renderable
	*/
	unsigned int GetCollisionQueryMask() const;
	
	/**
	* Controls whether this renderable casts shadows
	* @param castShadow Whether this renderable casts shadow 
	*/		
	void SetCastShadow(bool castShadow);
	
	/**
	* @returns Whether this renderable casts shadow 
	*/
	bool GetCastShadow() const;
	
	/**
	* Controls whether this renderable receives shadows
	* @param receiveShadow Whether this renderable receives shadows
	*/
	void SetReceiveShadow(bool receiveShadow);
	
	/**
	* @returns Whether this renderable recevies shadows
	*/
	bool GetReceiveShadow() const;
	
	/**
	* Sets the light mask of this renderable
	* @param lightMask The light mask
	*/
	void SetLightMask(unsigned int lightMask);
	
	/**
	* @returns The light mask of this renderable
	*/
	unsigned int GetLightMask() const;
	
	/**
	* Intersects a ray with this renderable
	* @param rayQuery The ray query object where the ray itself and the intersection results are stored
	*/
	virtual void IntersectRay(RayQuery &rayQuery);		
	
	/**
	* Renders debug geometry of this renderable
	* @param debugRenderer The debug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
		
protected:

	Renderable(const Renderable &other);

	virtual void UpdateWorldBoundingBox() = 0;

	/**
	* Updates the transformation of the renderable parts
	*/
	virtual void UpdateRenderableParts();

	/**
	* This function is called when the scene containing the node which owns this component changes
	* @param oldScene The previous scene this component was in
	*/
	virtual void SceneChanged(Scene *oldScene);

	/**
	* This function is called when the renderable is enabled / disabled
	*/
	virtual void EnabledChanged();

	/**
	* This function is called when the node owning this component is changed
	*/
	virtual void NodeChanged();

	/**
	* This function is called when the node's ownig this renderable transformation has changed
	*/
	
	// Functions implementing the interface needed to be able to insert renderables into an octree
	OctreeNode<Renderable> *GetOctreeNode();
	void SetOctreeNode(OctreeNode<Renderable> *octreeNode);
	bool GetOctreeNodeNeedsUpdate() const;
	void SetOctreeNodeNeedsUpdate(bool octreeNodeNeedsUpdate);
	virtual void MarkedDirty();

	std::vector<RenderablePart> renderableParts;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;	
	unsigned int collisionQueryMask;
	unsigned int lightMask;
	bool castShadow;
	bool receiveShadow;

	OctreeNode<Renderable> *octreeNode;
	bool octreeNodeNeedsUpdate;
private:

};

}