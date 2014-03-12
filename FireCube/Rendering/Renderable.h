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
};

class FIRECUBE_API Renderable : public Component
{	
public:
	Renderable(Engine *engine);
	virtual ~Renderable();
	const std::vector<RenderablePart> &GetRenderableParts() const;
	BoundingBox GetWorldBoundingBox();
	void SetScene(Scene *scene);
	void SetQueryIntersection(bool queryIntersection);
	bool GetQueryIntersection() const;

	virtual void IntersectRay(RayQuery &rayQuery);
	virtual void UpdateRenderableParts();
	virtual void NodeChanged();	
	virtual void MarkedDirty();
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
protected:
	virtual void UpdateWorldBoundingBox() = 0;

	std::vector<RenderablePart> renderableParts;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
	Scene *scene;
	bool queryIntersection;
private:

};

}