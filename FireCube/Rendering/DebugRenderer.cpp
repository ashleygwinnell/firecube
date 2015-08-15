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
		linesVertexBuffer->LoadData(lines.data(), lines.size(), VertexAttributeType::POSITION | VertexAttributeType::COLOR, BufferType::DYNAMIC);
		linesGeometry->SetPrimitiveCount(lines.size() / 4);
		linesGeometry->Update();
		linesGeometry->Render();
		lines.clear();
	}
}

void DebugRenderer::AddLine(const vec3 &start, const vec3 &end, const vec3 &color)
{
	lines.push_back({ start, vec4(color, 1.0f) });
	lines.push_back({ end, vec4(color, 1.0f) });	
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

void DebugRenderer::AddSphere(vec3 center, float radius, unsigned int rings, unsigned int columns, const vec3 &color)
{	
	const float fDeltaRingAngle = (float)PI / rings;
	const float fDeltacolumnAngle = 2 * (float)PI / columns;
	
	for (unsigned int ring = 0; ring < rings; ring++)
	{
		const float r0 = std::sin(ring * fDeltaRingAngle);
		const float y0 = std::cos(ring * fDeltaRingAngle);
		const float r1 = std::sin((ring + 1) * fDeltaRingAngle);
		const float y1 = std::cos((ring + 1) * fDeltaRingAngle);

		for (unsigned column = 0; column < columns; column++)
		{
			vec3 pos00(-r0 * std::sin(column * fDeltacolumnAngle), y0, -r0 * std::cos(column * fDeltacolumnAngle));
			vec3 pos10(-r0 * std::sin((column + 1) * fDeltacolumnAngle), y0, -r0 * std::cos((column + 1) * fDeltacolumnAngle));
			vec3 pos01(-r1 * std::sin(column * fDeltacolumnAngle), y1, -r1 * std::cos(column * fDeltacolumnAngle));
			vec3 pos11(-r1 * std::sin((column + 1) * fDeltacolumnAngle), y1, -r1 * std::cos((column + 1) * fDeltacolumnAngle));
			pos00.Normalize();
			pos10.Normalize();
			pos01.Normalize();
			pos11.Normalize();
			AddLine(center + pos00 * radius, center + pos10 * radius, color);
			AddLine(center + pos10 * radius, center + pos11 * radius, color);
			AddLine(center + pos11 * radius, center + pos01 * radius, color);
			AddLine(center + pos01 * radius, center + pos00 * radius, color);
		}
	}	
}