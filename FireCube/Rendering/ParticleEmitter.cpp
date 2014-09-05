#include <sstream>

#include "Rendering/ParticleEmitter.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderTemplate.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/VertexBuffer.h"
#include "Geometry/Material.h"

using namespace FireCube;

ParticleEmitter::ParticleEmitter(Engine *engine) : Renderable(engine)
{
	updateShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/particleUpdate.vert")->GenerateShader("");
	updateShader->SetOutputAttributes({"outPosition", "outVelocity", "outAge"});
	const int numberOfParticles = 1000000;
	std::vector<float> particleData(numberOfParticles * 7);
	for (int i = 0; i < numberOfParticles; ++i)
	{
		particleData[i * 7 + 0] = RangedRandom(-1, 1.0f); // Position
		particleData[i * 7 + 1] = RangedRandom(0, 1.0f);
		particleData[i * 7 + 2] = RangedRandom(-1, 1.0f);
		vec3 velocity = vec3(RangedRandom(-1, 1), RangedRandom(-1, 1), RangedRandom(-1, 1)).Normalized() * 0.005f;
		particleData[i * 7 + 3] = velocity.x; // Veloctiy
		particleData[i * 7 + 4] = velocity.y;
		particleData[i * 7 + 5] = velocity.z;
		particleData[i * 7 + 6] = 0.0f; // Age
		
	}
	for (int i = 0; i < 2; ++i)
	{
		particleBuffers[i] = new VertexBuffer(engine->GetRenderer());
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::POSITION, sizeof(float) * 3); // Position
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, sizeof(float) * 3);   // Velocity
		particleBuffers[i]->AddVertexAttribute(VertexAttributeType::CUSTOM, sizeof(float));		  // Age
		particleBuffers[i]->LoadData(&particleData[0], numberOfParticles, STREAM);
	}

	geometry = new Geometry(engine->GetRenderer());
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->SetPrimitiveType(PrimitiveType::POINTS);
	geometry->SetPrimitiveCount(numberOfParticles);
	geometry->Update();

	renderableParts.resize(1);
	renderableParts[0].geometry = geometry;
	renderableParts[0].material = engine->GetResourceCache()->GetResource<Material>("Materials/ParticleNoTexture.xml");
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
	engine->GetRenderer()->SetShaders(updateShader, nullptr);
	glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffers[1]->GetObjectId());
	glBeginTransformFeedback(GL_POINTS);		
	geometry->Render();
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	std::swap(particleBuffers[0], particleBuffers[1]);
	geometry->SetVertexBuffer(particleBuffers[0]);
	geometry->Update();

	for (auto &i : renderableParts)
	{
		i.transformation = node->GetWorldTransformation();
	}
}