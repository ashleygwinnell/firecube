#include <sstream>

#include "Scene/ParticleEmitter.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderTemplate.h"
#include "Rendering/DebugRenderer.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Material.h"
#include "Core/Events.h"

using namespace FireCube;

ParticleEmitter::ParticleEmitter(Engine *engine, unsigned int numberOfParticles, Material *material) : Renderable(engine), minLifeTime(2.0f), maxLifeTime(2.0f), minSpeed(1.0f), maxSpeed(1.0f), 
				 numberOfParticles(numberOfParticles), needToReset(true), emitterShape(ParticleEmitterShape::SPHERE), radius(1.0f), emissionRate(numberOfParticles / 10), prewarm(false)
{
	Init(numberOfParticles, material);
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &other) : Renderable(other), minLifeTime(other.minLifeTime), maxLifeTime(other.maxLifeTime), minSpeed(other.minSpeed), maxSpeed(other.maxSpeed),
																 numberOfParticles(other.numberOfParticles), needToReset(true), emitterShape(other.emitterShape), box(other.box), radius(other.radius), 
																 boundingBox(other.boundingBox), emissionRate(other.emissionRate), prewarm(other.prewarm)
{
	Init(numberOfParticles, other.renderableParts[0].material);	
}

void ParticleEmitter::Init(unsigned int numberOfParticles, Material *material)
{
	updateShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/particleUpdate.vert")->GenerateShader("");
	updateShader->SetOutputAttributes({ "outPosition", "outVelocity", "outLife", "outLifeTime" });
	for (int i = 0; i < 2; ++i)
	{
		particleBuffers[i] = new VertexBuffer(engine->GetRenderer());
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::POSITION, VertexAttributeDataType::FLOAT, 3); // Position
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 3);   // Velocity
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Life
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Life time
	}

	geometry = new Geometry(engine->GetRenderer());
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->SetPrimitiveType(PrimitiveType::POINTS);
	geometry->SetPrimitiveCount(numberOfParticles);

	renderableParts.resize(1);
	renderableParts[0].geometry = geometry;
	renderableParts[0].material = material;
	castShadow = false;

	SubscribeToEvent(Events::Update, &ParticleEmitter::Update);
}

ParticleEmitter::~ParticleEmitter()
{
	// First buffer is being deleted by the geometry
	delete particleBuffers[1];
}

void ParticleEmitter::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
	MarkedDirty();
}

void ParticleEmitter::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());
}

void ParticleEmitter::IntersectRay(RayQuery &rayQuery)
{
	
}

void ParticleEmitter::UpdateRenderableParts()
{	
	for (auto &i : renderableParts)
	{
		i.transformation = node->GetWorldTransformation();
	}
}

void ParticleEmitter::Reset()
{	
	emissionLeftOver = 0;
	particleLife.resize(numberOfParticles);	

	std::vector<float> particleData(numberOfParticles * particleDataSize);
	for (unsigned int i = 0; i < numberOfParticles; ++i)
	{		
		*((vec3 *)&particleData[i * particleDataSize]) = vec3(0.0f);
		*((vec3 *)&particleData[i * particleDataSize + 3]) = vec3(0.0f);
		
		float life = -1;
		particleData[i * particleDataSize + 6] = life; // Life
		particleData[i * particleDataSize + 7] = 1;  // Life time
		
		particleLife[i] = life;		
	}	

	for (unsigned int i = 0; i < numberOfParticles; ++i)
	{
		deadParticles.push_back(i);		
	}

	for (int i = 0; i < 2; ++i)
	{		
		particleBuffers[i]->LoadData(&particleData[0], numberOfParticles, BufferType::STREAM);
	}

	geometry->Update();

	if (prewarm)
	{
		Prewarm();
	}	
}

Component *ParticleEmitter::Clone() const
{
	ParticleEmitter *clone = new ParticleEmitter(*this);
	return clone;
}

void ParticleEmitter::SetMaterial(Material *material)
{
	renderableParts[0].material = material;
}

Material *ParticleEmitter::GetMaterial() const
{
	return renderableParts[0].material;
}

void ParticleEmitter::Update(float time)
{
	if (!IsEnabled())
		return;
	
	if (needToReset)
	{
		Reset();
		needToReset = false;
	}

	float particlesToEmitExact = (float)emissionRate * time + emissionLeftOver;
	unsigned int particlesToEmit = (unsigned int)std::floor(particlesToEmitExact);
	emissionLeftOver = particlesToEmitExact - particlesToEmit;
	EmitParticles(particlesToEmit);

	Program *program = engine->GetRenderer()->SetShaders(updateShader, nullptr);
	program->SetUniform(PARAM_TIME_STEP, time);	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffers[1]->GetObjectId());
	glBeginTransformFeedback(GL_POINTS);
	geometry->Render();
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	std::swap(particleBuffers[0], particleBuffers[1]);
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->Update();

	for (unsigned int i = 0; i < numberOfParticles; ++i)
	{
		if (particleLife[i] > 0)
		{
			particleLife[i] -= time;
			if (particleLife[i] < 0)
			{
				deadParticles.push_back(i);
			}
		}
	}	
}

void ParticleEmitter::EmitParticles(unsigned int count)
{	
	count = std::min(count, deadParticles.size());
	if (count > 0)
	{
		float *data = (float *)particleBuffers[0]->Lock();
		for (unsigned int i = 0; i < count; ++i)
		{
			unsigned int particleIndex = deadParticles[i];

			float *particleData = data + particleIndex * particleDataSize;
			vec3 pos, direction;
			RandomPositionAndDirection(pos, direction);
			*((vec3 *)particleData) = pos;
			*((vec3 *)(particleData + 3)) = direction * RangedRandom(minSpeed, maxSpeed);

			float life = RangedRandom(minLifeTime, maxLifeTime);
			particleData[6] = particleData[7] = particleLife[particleIndex] = life;
		}
		particleBuffers[0]->Unlock();
		deadParticles.erase(deadParticles.begin(), deadParticles.begin() + count);
	}
}

void ParticleEmitter::SetBoxEmitter(vec3 box)
{
	emitterShape = ParticleEmitterShape::BOX;
	this->box = box;
}

void ParticleEmitter::SetSphereEmitter(float radius)
{
	emitterShape = ParticleEmitterShape::SPHERE;
	this->radius = radius;
}

void ParticleEmitter::SetEmissionRate(unsigned int emissionRate)
{
	this->emissionRate = emissionRate;
}

void ParticleEmitter::SetLifeTime(float minLifeTime, float maxLifeTime)
{
	this->minLifeTime = minLifeTime;
	this->maxLifeTime = maxLifeTime;
}

void ParticleEmitter::RandomPositionAndDirection(vec3 &position, vec3 &direction) const
{
	if (emitterShape == ParticleEmitterShape::BOX)
	{
		position = vec3(RangedRandom(-box.x * 0.5f, box.x * 0.5f), RangedRandom(-box.y * 0.5f, box.y * 0.5f), RangedRandom(-box.z * 0.5f, box.z * 0.5f));
		direction = vec3(0, 0, 1);
	}
	else if (emitterShape == ParticleEmitterShape::SPHERE)
	{
		float ang0 = RangedRandom(0, PI * 2.0f);
		float ang1 = RangedRandom(0, PI * 2.0f);
		float rad = RangedRandom(0, radius);
		
		direction.FromAngles(ang0, ang1);
		direction.Normalize();
		position = direction * radius;
	}	
}

void ParticleEmitter::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	if (emitterShape == ParticleEmitterShape::BOX)
	{		
		vec3 p[8];
		p[0] = node->GetWorldTransformation() * vec3(-box.x * 0.5f, -box.y * 0.5f, -box.z * 0.5f);
		p[1] = node->GetWorldTransformation() * vec3(box.x * 0.5f, -box.y * 0.5f, -box.z * 0.5f);
		p[2] = node->GetWorldTransformation() * vec3(box.x * 0.5f, box.y * 0.5f, -box.z * 0.5f);
		p[3] = node->GetWorldTransformation() * vec3(-box.x * 0.5f, box.y * 0.5f, -box.z * 0.5f);
		p[4] = node->GetWorldTransformation() * vec3(-box.x * 0.5f, -box.y * 0.5f, box.z * 0.5f);
		p[5] = node->GetWorldTransformation() * vec3(box.x * 0.5f, -box.y * 0.5f, box.z * 0.5f);
		p[6] = node->GetWorldTransformation() * vec3(box.x * 0.5f, box.y * 0.5f, box.z * 0.5f);
		p[7] = node->GetWorldTransformation() * vec3(-box.x * 0.5f, box.y * 0.5f, box.z * 0.5f);
		debugRenderer->AddLine(p[0], p[1], vec3(0, 1, 0));
		debugRenderer->AddLine(p[1], p[2], vec3(0, 1, 0));
		debugRenderer->AddLine(p[2], p[3], vec3(0, 1, 0));
		debugRenderer->AddLine(p[3], p[0], vec3(0, 1, 0));
		debugRenderer->AddLine(p[4], p[5], vec3(0, 1, 0));
		debugRenderer->AddLine(p[5], p[6], vec3(0, 1, 0));
		debugRenderer->AddLine(p[6], p[7], vec3(0, 1, 0));
		debugRenderer->AddLine(p[7], p[4], vec3(0, 1, 0));
		debugRenderer->AddLine(p[0], p[4], vec3(0, 1, 0));
		debugRenderer->AddLine(p[1], p[5], vec3(0, 1, 0));
		debugRenderer->AddLine(p[2], p[6], vec3(0, 1, 0));
		debugRenderer->AddLine(p[3], p[7], vec3(0, 1, 0));				
	}
	else if (emitterShape == ParticleEmitterShape::SPHERE)
	{
		debugRenderer->AddSphere(node->GetWorldPosition(), radius, 16, 16, vec3(0.0f, 1.0f, 0.0f));
	}	
}

void ParticleEmitter::SetSpeed(float minSpeed, float maxSpeed)
{
	this->minSpeed = minSpeed;
	this->maxSpeed = maxSpeed;
}

void ParticleEmitter::SetPrewarm(bool prewarm)
{
	this->prewarm = prewarm;
}

void FireCube::ParticleEmitter::Prewarm()
{
	float timeStep = 0.1f;
	float totalTime = maxLifeTime;
	float time = 0.0f;
	while (time < totalTime)
	{
		float particlesToEmitExact = (float)emissionRate * timeStep + emissionLeftOver;
		unsigned int particlesToEmit = (unsigned int)std::floor(particlesToEmitExact);
		emissionLeftOver = particlesToEmitExact - particlesToEmit;
		EmitParticles(particlesToEmit);

		Program *program = engine->GetRenderer()->SetShaders(updateShader, nullptr);
		program->SetUniform(PARAM_TIME_STEP, timeStep);
		glEnable(GL_RASTERIZER_DISCARD);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffers[1]->GetObjectId());
		glBeginTransformFeedback(GL_POINTS);
		geometry->Render();
		glEndTransformFeedback();
		glDisable(GL_RASTERIZER_DISCARD);
		std::swap(particleBuffers[0], particleBuffers[1]);
		geometry->SetVertexBuffer(particleBuffers[0]);
		geometry->Update();

		for (unsigned int i = 0; i < numberOfParticles; ++i)
		{
			if (particleLife[i] > 0)
			{
				particleLife[i] -= timeStep;
				if (particleLife[i] < 0)
				{
					deadParticles.push_back(i);
				}
			}
		}		

		time += timeStep;
	}
}
