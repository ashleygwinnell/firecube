#pragma once

#include "Scene/Renderable.h"

namespace FireCube
{

class Shader;

enum class ParticleEmitterShape
{
	BOX, SPHERE
};

class ParticleEmitter : public Renderable
{
	FIRECUBE_OBJECT(ParticleEmitter)
public:
	ParticleEmitter(Engine *engine, unsigned int numberOfParticles, Material *material);
	~ParticleEmitter();

	virtual void IntersectRay(RayQuery &rayQuery);
	void Reset();
	virtual Component *Clone() const;
	void SetMaterial(Material *material);
	Material *GetMaterial() const;
	void SetBoundingBox(BoundingBox boundingBox);	
	void SetBoxEmitter(vec3 box);
	void SetSphereEmitter(float radius);
	void SetEmissionRate(unsigned int emissionRate);
	void SetLifeTime(float lifeTime);
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);

protected:	
	virtual void UpdateWorldBoundingBox();
	virtual void UpdateRenderableParts();	

private:

	const int particleDataSize = 8;

	ParticleEmitter(const ParticleEmitter &other);
	void Update(float time);
	void EmitParticles(unsigned int count);
	void Init(unsigned int numberOfParticles, Material *material);
	inline void RandomPositionAndVelocity(vec3 &position, vec3 &velocity) const;	

	BoundingBox boundingBox;
	Shader *updateShader;
	VertexBuffer *particleBuffers[2];	
	SharedPtr<Geometry> geometry;
	float lifeTime;
	unsigned int numberOfParticles;
	std::vector<float> particleLife;
	std::vector<unsigned int> deadParticles;
	unsigned int emissionRate;
	bool needToReset;
	ParticleEmitterShape emitterShape;
	float radius;
	vec3 box;
	float emissionLeftOver;
};

}