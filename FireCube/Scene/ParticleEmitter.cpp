#include <sstream>

#include "Scene/ParticleEmitter.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderTemplate.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Material.h"
#include "Core/Events.h"

using namespace FireCube;

ParticleEmitter::ParticleEmitter(Engine *engine, unsigned int numberOfParticles, Material *material) : Renderable(engine), lifeTime(2.0f), numberOfParticles(numberOfParticles)
{
	updateShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/particleUpdate.vert")->GenerateShader("");
	updateShader->SetOutputAttributes({"outPosition", "outVelocity", "outAge", "outLifeTime"});
	for (int i = 0; i < 2; ++i)
	{
		particleBuffers[i] = new VertexBuffer(engine->GetRenderer());
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::POSITION, VertexAttributeDataType::FLOAT, 3); // Position
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 3);   // Velocity
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Age
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Life time
	}

	Reset();

	geometry = new Geometry(engine->GetRenderer());
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->SetPrimitiveType(PrimitiveType::POINTS);
	geometry->SetPrimitiveCount(numberOfParticles);
	geometry->Update();

	renderableParts.resize(1);
	renderableParts[0].geometry = geometry;
	renderableParts[0].material = material;
	castShadow = false;

	SubscribeToEvent(Events::Update, &ParticleEmitter::Update);
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &other) : Renderable(other), lifeTime(other.lifeTime), numberOfParticles(other.numberOfParticles)
{
	updateShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/particleUpdate.vert")->GenerateShader("");
	updateShader->SetOutputAttributes({"outPosition", "outVelocity", "outAge", "outLifeTime"});
	for (int i = 0; i < 2; ++i)
	{
		particleBuffers[i] = new VertexBuffer(engine->GetRenderer());
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::POSITION, VertexAttributeDataType::FLOAT, 3); // Position
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 3);   // Velocity
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Age
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, VertexAttributeDataType::FLOAT, 1);	  // Life time
	}

	Reset();

	geometry = new Geometry(engine->GetRenderer());
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->SetPrimitiveType(PrimitiveType::POINTS);
	geometry->SetPrimitiveCount(numberOfParticles);
	geometry->Update();

	renderableParts.resize(1);
	renderableParts[0].geometry = geometry;
	renderableParts[0].material = other.renderableParts[0].material;
	castShadow = false;
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
	std::vector<float> particleData(numberOfParticles * 8);
	for (unsigned int i = 0; i < numberOfParticles; ++i)
	{
		particleData[i * 8 + 0] = 0;// RangedRandom(-1, 1.0f); // Position
		particleData[i * 8 + 1] = 0;// RangedRandom(0, 1.0f);
		particleData[i * 8 + 2] = 0;// RangedRandom(-1, 1.0f);
		vec3 velocity = vec3(RangedRandom(-1, 1), RangedRandom(-1, 1), RangedRandom(-1, 1)).Normalized() * RangedRandom(1, 1.5f);
		particleData[i * 8 + 3] = velocity.x; // Veloctiy
		particleData[i * 8 + 4] = velocity.y;
		particleData[i * 8 + 5] = velocity.z;
		particleData[i * 8 + 6] = 0.0f;
		particleData[i * 8 + 7] = RangedRandom(0.0f, lifeTime); // Life time
	}
	for (int i = 0; i < 2; ++i)
	{		
		particleBuffers[i]->LoadData(&particleData[0], numberOfParticles, BufferType::STREAM);
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
	Program *program = engine->GetRenderer()->SetShaders(updateShader, nullptr);
	program->SetUniform(PARAM_TIME_STEP, engine->GetRenderer()->GetTimeStep());	
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffers[1]->GetObjectId());
	glBeginTransformFeedback(GL_POINTS);
	geometry->Render();
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	std::swap(particleBuffers[0], particleBuffers[1]);
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->Update();
}
