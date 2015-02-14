#pragma once

#include "Renderable.h"

namespace FireCube
{

class Shader;

class ParticleEmitter : public Renderable
{
	OBJECT(ParticleEmitter)
public:
	ParticleEmitter(Engine *engine, int numberOfParticles, Material *material);
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
	float lifeTime;
};

}