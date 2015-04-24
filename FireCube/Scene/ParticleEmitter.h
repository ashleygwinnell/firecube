#pragma once

#include "Scene/Renderable.h"

namespace FireCube
{

class Shader;

class ParticleEmitter : public Renderable
{
	OBJECT(ParticleEmitter)
public:
	ParticleEmitter(Engine *engine, unsigned int numberOfParticles, Material *material);
	~ParticleEmitter();

	virtual void IntersectRay(RayQuery &rayQuery);
	void Reset();
	virtual Component *Clone() const;
	void SetMaterial(Material *material);
	Material *GetMaterial() const;
protected:
	void SetBoundingBox(BoundingBox boundingBox);
	virtual void UpdateWorldBoundingBox();
	virtual void UpdateRenderableParts();
private:

	ParticleEmitter(const ParticleEmitter &other);

	BoundingBox boundingBox;
	Shader *updateShader;
	VertexBuffer *particleBuffers[2];	
	SharedPtr<Geometry> geometry;
	float lifeTime;
	unsigned int numberOfParticles;
};

}