#include "ParticleEmitterDescriptor.h"

using namespace FireCube;

#include "FireCube.h"

using namespace FireCube;

ParticleEmitterDescriptor::ParticleEmitterDescriptor() : ComponentDescriptor(ComponentType::PARTICLE_EMITTER), numberOfParticles(1000), emissionRate(100), lifeTime(2.0f), box(1.0f), 
														 radius(1.0f), shape(ParticleEmitterShape::BOX)
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

float ParticleEmitterDescriptor::GetLifeTime() const
{
	return lifeTime;
}

void ParticleEmitterDescriptor::SetLifeTime(float val)
{
	lifeTime = val;
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

ComponentDescriptor *ParticleEmitterDescriptor::Clone()
{
	ParticleEmitterDescriptor *clone = new ParticleEmitterDescriptor();
	
	return clone;
}
