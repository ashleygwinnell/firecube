#pragma once

#include <vector>
#include "Core/Component.h"
#include "Rendering/RenderQueue.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

class Geometry;

class RenderablePart
{
public:
	/**
	* The geometry of this job.
	*/
	Geometry *geometry;		

	/**
	* The transformation to apply to this job.
	*/
	mat4 transformation;	
};

class Renderable : public Component
{	
public:
	Renderable(Engine *engine);
	virtual ~Renderable();
	const std::vector<RenderablePart> &GetRenderableParts() const;
	BoundingBox GetWorldBoundingBox();

	virtual void UpdateRenderableParts();
	virtual void NodeChanged();	
	virtual void MarkedDirty();
protected:
	virtual void UpdateWorldBoundingBox() = 0;

	std::vector<RenderablePart> renderableParts;
	BoundingBox worldBoundingBox;
	bool worldBoundingBoxChanged;
private:

};

}