#pragma once

#include "Renderable.h"

namespace FireCube
{

class Shader;

class ParticleEmitter : public Renderable
{
public:
	ParticleEmitter(Engine *engine);
	~ParticleEmitter();

	virtual void IntersectRay(RayQuery &rayQuery);
protected:
	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	virtual void UpdateRenderableParts();
private:

	BoundingBox boundingBox;
	Shader *updateShader;
	VertexBuffer *particleBuffers[2];	
	SharedPtr<Geometry> geometry;
};

}