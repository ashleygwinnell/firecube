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

	if (component)
	{
		((ParticleEmitter *)component)->SetNumberOfParticles(val);
	}
}

unsigned int ParticleEmitterDescriptor::GetEmissionRate() const
{
	return emissionRate;
}

void ParticleEmitterDescriptor::SetEmissionRate(unsigned int val)
{
	emissionRate = val;

	if (component)
	{
		((ParticleEmitter *)component)->SetEmissionRate(val);
	}
}

std::string ParticleEmitterDescriptor::GetMaterial() const
{
	return material;
}

void ParticleEmitterDescriptor::SetMaterial(const std::string &val, FireCube::Engine *engine)
{
	material = val;

	if (component)
	{
		((ParticleEmitter *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(val));
	}
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

	if (component)
	{
		((ParticleEmitter *)component)->SetLifeTime(minLifeTime, maxLifeTime);
	}
}

void ParticleEmitterDescriptor::SetMaxLifeTime(float maxLifeTime)
{	
	this->maxLifeTime = maxLifeTime;

	if (component)
	{
		((ParticleEmitter *)component)->SetLifeTime(minLifeTime, maxLifeTime);
	}
}

void ParticleEmitterDescriptor::SetLifeTime(float minLifeTime, float maxLifeTime)
{
	this->minLifeTime = minLifeTime;
	this->maxLifeTime = maxLifeTime;

	if (component)
	{
		((ParticleEmitter *)component)->SetLifeTime(minLifeTime, maxLifeTime);
	}
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

	if (component)
	{
		((ParticleEmitter *)component)->SetSpeed(minSpeed, maxSpeed);
	}
}

void ParticleEmitterDescriptor::SetMaxSpeed(float maxSpeed)
{	
	this->maxSpeed = maxSpeed;

	if (component)
	{
		((ParticleEmitter *)component)->SetSpeed(minSpeed, maxSpeed);
	}
}

void ParticleEmitterDescriptor::SetSpeed(float minSpeed, float maxSpeed)
{
	this->minSpeed = minSpeed;
	this->maxSpeed = maxSpeed;

	if (component)
	{
		((ParticleEmitter *)component)->SetSpeed(minSpeed, maxSpeed);
	}
}

bool ParticleEmitterDescriptor::GetPrewarm() const
{
	return prewarm;
}

void ParticleEmitterDescriptor::SetPrewarm(bool val)
{
	prewarm = val;

	if (component)
	{
		((ParticleEmitter *)component)->SetPrewarm(val);
	}
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

	particleEmitter->SetBoundingBox(BoundingBox(vec3(-1000.0f), vec3(1000.0f)));	
	particleEmitter->SetMaterial(engine->GetResourceCache()->GetResource<Material>(material));
	particleEmitter->SetEmissionRate(emissionRate);
	particleEmitter->SetLifeTime(minLifeTime, maxLifeTime);
	particleEmitter->SetPrewarm(prewarm);
	particleEmitter->SetSimulationSpace(simulationSpace);
	particleEmitter->SetSpeed(minSpeed, maxSpeed);
	particleEmitter->SetEnabled(false);
	
	component = particleEmitter;
}

void ParticleEmitterDescriptor::SetSimulationSpace(ParticleEmitterSimulationSpace simulationSpace)
{
	this->simulationSpace = simulationSpace;

	if (component)
	{
		((ParticleEmitter *)component)->SetSimulationSpace(simulationSpace);
	}
}

ParticleEmitterSimulationSpace ParticleEmitterDescriptor::GetSimulationSpace() const
{
	return simulationSpace;
}

void ParticleEmitterDescriptor::SetEnabled(bool enable)
{
	if (component)
	{
		((ParticleEmitter *)component)->SetEnabled(enable);		
	}
}

bool ParticleEmitterDescriptor::IsEnabled() const
{
	if (component)
	{
		return ((ParticleEmitter *)component)->IsEnabled();
	}

	return false;
}

void ParticleEmitterDescriptor::Reset()
{
	if (component)
	{
		((ParticleEmitter *)component)->Reset();
	}
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
