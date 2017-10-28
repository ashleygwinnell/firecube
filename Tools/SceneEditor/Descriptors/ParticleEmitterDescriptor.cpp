#include "ParticleEmitterDescriptor.h"

using namespace FireCube;

#include "FireCube.h"

using namespace FireCube;

ParticleEmitterDescriptor::ParticleEmitterDescriptor() : ComponentDescriptor(ComponentType::PARTICLE_EMITTER), numberOfParticles(1000), emissionRate(100), minLifeTime(2.0f), maxLifeTime(2.0f),
														 box(1.0f), minSpeed(1.0f), maxSpeed(1.0f), radius(1.0f), shape(ParticleEmitterShape::BOX), prewarm(false), simulationSpace(ParticleEmitterSimulationSpace::LOCAL)
{

}

ParticleEmitterDescriptor::~ParticleEmitterDescriptor()
{

}

void ParticleEmitterDescriptor::SetBoxEmitter(vec3 box)
{
	shape = ParticleEmitterShape::BOX;	
	this->box = box;

	if (component)
	{
		((ParticleEmitter *)component)->SetBoxEmitter(box);
	}
}

void ParticleEmitterDescriptor::SetSphereEmitter(float radius)
{
	shape = ParticleEmitterShape::SPHERE;
	this->radius = radius;

	if (component)
	{
		((ParticleEmitter *)component)->SetSphereEmitter(radius);
	}
}

ParticleEmitterShape ParticleEmitterDescriptor::GetEmitterShape() const
{
	return shape;
}

vec3 ParticleEmitterDescriptor::GetBox() const
{
	return box;
}

float ParticleEmitterDescriptor::GetRadius() const
{
	return radius;
}

unsigned int ParticleEmitterDescriptor::GetNumberOfParticles() const
{
	return numberOfParticles;
}

void ParticleEmitterDescriptor::SetNumberOfParticles(unsigned int val)
{
	numberOfParticles = val;
}

unsigned int ParticleEmitterDescriptor::GetEmissionRate() const
{
	return emissionRate;
}

void ParticleEmitterDescriptor::SetEmissionRate(unsigned int val)
{
	emissionRate = val;
}

std::string ParticleEmitterDescriptor::GetMaterial() const
{
	return material;
}

void ParticleEmitterDescriptor::SetMaterial(std::string val)
{
	material = val;
}

float ParticleEmitterDescriptor::GetMinLifeTime() const
{
	return minLifeTime;
}

float ParticleEmitterDescriptor::GetMaxLifeTime() const
{
	return maxLifeTime;
}

void ParticleEmitterDescriptor::SetMinLifeTime(float minLifeTime)
{
	this->minLifeTime = minLifeTime;
}

void ParticleEmitterDescriptor::SetMaxLifeTime(float maxLifeTime)
{	
	this->maxLifeTime = maxLifeTime;
}

void ParticleEmitterDescriptor::SetLifeTime(float minLifeTime, float maxLifeTime)
{
	this->minLifeTime = minLifeTime;
	this->maxLifeTime = maxLifeTime;
}

float ParticleEmitterDescriptor::GetMinSpeed() const
{
	return minSpeed;
}

float ParticleEmitterDescriptor::GetMaxSpeed() const
{
	return maxSpeed;
}

void ParticleEmitterDescriptor::SetMinSpeed(float minSpeed)
{
	this->minSpeed = minSpeed;
}

void ParticleEmitterDescriptor::SetMaxSpeed(float maxSpeed)
{	
	this->maxSpeed = maxSpeed;
}

void ParticleEmitterDescriptor::SetSpeed(float minSpeed, float maxSpeed)
{
	this->minSpeed = minSpeed;
	this->maxSpeed = maxSpeed;
}

bool ParticleEmitterDescriptor::GetPrewarm() const
{
	return prewarm;
}

void ParticleEmitterDescriptor::SetPrewarm(bool val)
{
	prewarm = val;
}

void ParticleEmitterDescriptor::CreateComponent(Node *node, Engine *engine)
{
	auto particleEmitter = node->CreateComponent<ParticleEmitter>(numberOfParticles, nullptr);
	if (shape == ParticleEmitterShape::BOX)
	{
		particleEmitter->SetBoxEmitter(box);
	}
	else if (shape == ParticleEmitterShape::SPHERE)
	{
		particleEmitter->SetSphereEmitter(radius);
	}
	component = particleEmitter;
}

void ParticleEmitterDescriptor::SetSimulationSpace(ParticleEmitterSimulationSpace simulationSpace)
{
	this->simulationSpace = simulationSpace;
}

ParticleEmitterSimulationSpace ParticleEmitterDescriptor::GetSimulationSpace() const
{
	return simulationSpace;
}

ComponentDescriptor *ParticleEmitterDescriptor::Clone()
{
	ParticleEmitterDescriptor *clone = new ParticleEmitterDescriptor();
	clone->shape = shape;
	clone->box = box;
	clone->radius = radius;
	clone->numberOfParticles = numberOfParticles;
	clone->emissionRate = emissionRate;
	clone->material = material;
	clone->minLifeTime = minLifeTime;
	clone->maxLifeTime = maxLifeTime;
	clone->minSpeed = minSpeed;
	clone->maxSpeed = maxSpeed;
	clone->prewarm = prewarm;
	clone->simulationSpace = simulationSpace;

	return clone;
}
