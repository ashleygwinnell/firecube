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
	* The transformation to apply to this part.
	*/
	mat4 transformation;

	mat4 *skinMatrices;

	unsigned int skinMatricesCount;
};

class FIRECUBE_API Renderable : public Component
{	
	OBJECT(Renderable)
public:
	Renderable(Engine *engine);
	virtual ~Renderable();
	const std::vector<RenderablePart> &GetRenderableParts() const;
	BoundingBox GetWorldBoundingBox();
	void SetScene(Scene *scene);
	void SetCollisionQueryMask(unsigned int collisionQueryMask);
	unsigned int GetCollisionQueryMask() const;
	void SetCastShadow(bool castShadow);
	bool GetCastShadow() const;
	void SetReceiveShadow(bool receiveShadow);
	bool GetReceiveShadow() const;	
	void SetLightMask(unsigned int lightMask);
	unsigned int GetLightMask() const;

	virtual void IntersectRay(RayQuery &rayQuery);
	virtual void UpdateRenderableParts();
	virtual void NodeChanged();	
	virtual void SceneChanged(Scene *oldScene);
	virtual void EnabledChanged();
	virtual void MarkedDirty();
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	OctreeNode<Renderable> *GetOctreeNode();
	void SetOctreeNode(OctreeNode<Renderable> *octreeNode);
	bool GetOctreeNodeNeedsUpdate() const;
	void SetOctreeNodeNeedsUpdate(bool octreeNodeNeedsUpdate);
protected:

	Renderable(const Renderable &other);

	virtual void UpdateWorldBoundingBox() = 0;

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