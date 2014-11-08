#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/ShaderTemplate.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/Renderer.h"
#include "Geometry/Geometry.h"
#include "Rendering/VertexBuffer.h"
using namespace FireCube;

DebugRenderer::DebugRenderer(Engine *engine) : engine(engine)
{
	
}

DebugRenderer::~DebugRenderer()
{
	delete linesGeometry;
}

void DebugRenderer::Initialize()
{
	linesGeometry = new Geometry(engine->GetRenderer());
	linesVertexBuffer = new VertexBuffer(engine->GetRenderer());
	linesGeometry->SetVertexBuffer(linesVertexBuffer);
	linesGeometry->SetPrimitiveType(PrimitiveType::LINES);
	linesGeometry->SetPrimitiveCount(0);
}
void DebugRenderer::Render(Camera *camera)
{
	if (lines.empty() == false)
	{
		Renderer *renderer = engine->GetRenderer();
		Shader *vs = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/solidColor.vert")->GenerateShader("");
		Shader *fs = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/solidColor.frag")->GenerateShader("");
		renderer->SetDepthTest(DepthTest::LESS);
		renderer->SetDepthWrite(true);
		renderer->SetBlendMode(BlendMode::REPLACE);
		renderer->SetShaders(vs, fs);
		renderer->UseCamera(camera);
		linesVertexBuffer->LoadData(&lines[0], lines.size() / 2, VertexAttributeType::POSITION | VertexAttributeType::COLOR, BufferType::DYNAMIC);
		linesGeometry->SetPrimitiveCount(lines.size() / 4);
		linesGeometry->Update();
		linesGeometry->Render();
		lines.clear();
	}
}

void DebugRenderer::AddLine(const vec3 &start, const vec3 &end, const vec3 &color)
{
	lines.push_back(start);
	lines.push_back(color);
	lines.push_back(end);
	lines.push_back(color);
}

void DebugRenderer::AddBoundingBox(const BoundingBox &boundingBox, const vec3 &color)
{
	vec3 min = boundingBox.GetMin();
	vec3 max = boundingBox.GetMax();
	AddLine(vec3(min.x, min.y, min.z), vec3(max.x, min.y, min.z), color);
	AddLine(vec3(max.x, min.y, min.z), vec3(max.x, min.y, max.z), color);
	AddLine(vec3(max.x, min.y, max.z), vec3(min.x, min.y, max.z), color);
	AddLine(vec3(min.x, min.y, max.z), vec3(min.x, min.y, min.z), color);

	AddLine(vec3(min.x, max.y, min.z), vec3(max.x, max.y, min.z), color);
	AddLine(vec3(max.x, max.y, min.z), vec3(max.x, max.y, max.z), color);
	AddLine(vec3(max.x, max.y, max.z), vec3(min.x, max.y, max.z), color);
	AddLine(vec3(min.x, max.y, max.z), vec3(min.x, max.y, min.z), color);

	AddLine(vec3(min.x, min.y, min.z), vec3(min.x, max.y, min.z), color);
	AddLine(vec3(max.x, min.y, min.z), vec3(max.x, max.y, min.z), color);
	AddLine(vec3(max.x, min.y, max.z), vec3(max.x, max.y, max.z), color);
	AddLine(vec3(min.x, min.y, max.z), vec3(min.x, max.y, max.z), color);
}