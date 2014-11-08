#include "Rendering/CustomGeometry.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Core/Engine.h"
#include "Rendering/VertexBuffer.h"
using namespace FireCube;

CustomGeometry::CustomGeometry(Engine *engine) : Renderable(engine), geometry(new Geometry(engine->GetRenderer())), vertexAttributes(VertexAttributeType::POSITION), currentVertex(0)
{
	renderableParts.resize(1);
	renderableParts[0].geometry = geometry;
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	geometry->SetVertexBuffer(vertexBuffer);
}

CustomGeometry::~CustomGeometry()
{	
	delete geometry;
}

void CustomGeometry::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());
}

void CustomGeometry::SetPrimitiveType(PrimitiveType primitiveType)
{
	geometry->SetPrimitiveType(primitiveType);
}

void CustomGeometry::AddVertex(vec3 position)
{	
	vertices.push_back(CustomVertex());
	vertices.back().position = position;
	currentVertex = vertices.size() - 1;
	boundingBox.Expand(position);
}

void CustomGeometry::SetNormal(vec3 normal)
{
	vertices[currentVertex].normal = normal;
	vertexAttributes |= VertexAttributeType::NORMAL;
}

void CustomGeometry::SetTexCoord(vec2 texcoord)
{
	vertices[currentVertex].texcoord = texcoord;
	vertexAttributes |= VertexAttributeType::TEXCOORD0;
}

void CustomGeometry::UpdateGeometry()
{
	VertexBuffer *vertexBuffer = geometry->GetVertexBuffer();
	unsigned int vertexSize = VertexBuffer::GetVertexSize(vertexAttributes);
	std::vector<char> vertexData(vertices.size() * vertexSize);
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		if ((vertexAttributes & VertexAttributeType::POSITION) == VertexAttributeType::POSITION)
		{
			*((vec3 *) &vertexData[currentOffset]) = vertices[i].position;
			currentOffset += VertexBuffer::GetAttributeSize(VertexAttributeType::POSITION);
		}
		if ((vertexAttributes & VertexAttributeType::NORMAL) == VertexAttributeType::NORMAL)
		{
			*((vec3 *) &vertexData[currentOffset]) = vertices[i].normal;
			currentOffset += VertexBuffer::GetAttributeSize(VertexAttributeType::NORMAL);
		}
		if ((vertexAttributes & VertexAttributeType::TEXCOORD0) == VertexAttributeType::TEXCOORD0)
		{
			*((vec2 *) &vertexData[currentOffset]) = vertices[i].texcoord;
			currentOffset += VertexBuffer::GetAttributeSize(VertexAttributeType::TEXCOORD0);
		}
	}	
	
	if (vertexData.empty() == false)
		vertexBuffer->LoadData(&vertexData[0], vertices.size(), vertexAttributes, BufferType::STATIC);

	switch (geometry->GetPrimitiveType())
	{
	case PrimitiveType::POINTS:
		geometry->SetPrimitiveCount(vertices.size());
		break;
	case PrimitiveType::LINES:
		geometry->SetPrimitiveCount(vertices.size() / 2);
		break;
	case PrimitiveType::TRIANGLES:
		geometry->SetPrimitiveCount(vertices.size() / 3);
		break;
	default:
		break;
	}
	geometry->Update();
	MarkedDirty();
}

void CustomGeometry::SetMaterial(Material *material)
{
	this->material = material;
	renderableParts[0].material = material;
}

void CustomGeometry::Clear()
{
	vertexAttributes = VertexAttributeType::POSITION;
	currentVertex = 0;
	boundingBox = BoundingBox();
	vertices.clear();
}