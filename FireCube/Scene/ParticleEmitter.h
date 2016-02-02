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
	void SetLifeTime(float minLifeTime, float maxLifeTime);
	void SetSpeed(float minSpeed, float maxSpeed);
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	void SetPrewarm(bool prewarm);

protected:	
	virtual void UpdateWorldBoundingBox();
	virtual void UpdateRenderableParts();	

private:

	const int particleDataSize = 8;

	ParticleEmitter(const ParticleEmitter &other);
	void Update(float time);
	void EmitParticles(unsigned int count);
	void Init(unsigned int numberOfParticles, Material *material);
	inline void RandomPositionAndDirection(vec3 &position, vec3 &direction) const;	
	void Prewarm();

	BoundingBox boundingBox;
	Shader *updateShader;
	VertexBuffer *particleBuffers[2];	
	SharedPtr<Geometry> geometry;
	float minLifeTime;
	float maxLifeTime;
	float minSpeed;
	float maxSpeed;
	unsigned int numberOfParticles;
	std::vector<float> particleLife;
	std::vector<unsigned int> deadParticles;
	unsigned int emissionRate;
	bool needToReset;
	ParticleEmitterShape emitterShape;
	float radius;
	vec3 box;
	float emissionLeftOver;
	bool prewarm;
};

}