#pragma once

#include "ComponentDescriptor.h"

class ParticleEmitterDescriptor : public ComponentDescriptor
{
public:
	ParticleEmitterDescriptor();
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine);
	virtual ComponentDescriptor *Clone();
	virtual ~ParticleEmitterDescriptor();

	void SetBoxEmitter(FireCube::vec3 box);
	void SetSphereEmitter(float radius);
	FireCube::ParticleEmitterShape GetEmitterShape() const;
	FireCube::vec3 GetBox() const;
	float GetRadius() const;

	unsigned int GetNumberOfParticles() const;
	void SetNumberOfParticles(unsigned int val);
	unsigned int GetEmissionRate() const;
	void SetEmissionRate(unsigned int val);
	std::string GetMaterial() const;
	void SetMaterial(std::string val);
	float GetMinLifeTime() const;
	float GetMaxLifeTime() const;
	void SetLifeTime(float minLifeTime, float maxLifeTime);
	float GetMinSpeed() const;
	float GetMaxSpeed() const;
	void SetSpeed(float minSpeed, float maxSpeed);
	bool GetPrewarm() const;
	void SetPrewarm(bool val);
private:
	FireCube::ParticleEmitterShape shape;
	FireCube::vec3 box;
	float radius;
	unsigned int numberOfParticles;
	unsigned int emissionRate;
	std::string material;
	float minLifeTime;
	float maxLifeTime;
	float minSpeed;
	float maxSpeed;
	bool prewarm;
};