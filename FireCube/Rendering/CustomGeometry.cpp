#include "Rendering/CustomGeometry.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Core/Engine.h"
#include "Rendering/VertexBuffer.h"
using namespace FireCube;

CustomGeometry::CustomGeometry(Engine *engine) : Renderable(engine), geometry(new Geometry(engine->GetRenderer())), vertexAttributes(VERTEX_ATTRIBUTE_POSITION), currentVertex(0)
{
	renderableParts.resize(1);
	renderableParts[0].geometry = geometry.get();
	VertexBufferPtr vertexBuffer(new VertexBuffer(engine->GetRenderer()));
	geometry->SetVertexBuffer(vertexBuffer);
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
}

void CustomGeometry::SetTexCoord(vec2 texcoord)
{
	vertices[currentVertex].texcoord = texcoord;
}

void CustomGeometry::UpdateGeometry()
{
	VertexBufferPtr vertexBuffer = geometry->GetVertexBuffer();
	unsigned int vertexSize = VertexBuffer::GetVertexSize(vertexAttributes);
	std::vector<char> vertexData(vertices.size() * vertexSize);
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		if (vertexAttributes & VERTEX_ATTRIBUTE_POSITION)
		{
			*((vec3 *) &vertexData[currentOffset]) = vertices[i].position;
			currentOffset += VertexBuffer::GetAttributeSize(VERTEX_ATTRIBUTE_POSITION);
		}
		if (vertexAttributes & VERTEX_ATTRIBUTE_NORMAL)
		{
			*((vec3 *) &vertexData[currentOffset]) = vertices[i].normal;
			currentOffset += VertexBuffer::GetAttributeSize(VERTEX_ATTRIBUTE_NORMAL);
		}
		if (vertexAttributes & VERTEX_ATTRIBUTE_TEXCOORD0)
		{
			*((vec2 *) &vertexData[currentOffset]) = vertices[i].texcoord;
			currentOffset += VertexBuffer::GetAttributeSize(VERTEX_ATTRIBUTE_TEXCOORD0);
		}
	}	
	vertexBuffer->LoadData(&vertexData[0], vertices.size(), vertexAttributes, STATIC);
	switch (geometry->GetPrimitiveType())
	{
	case POINTS:
		geometry->SetPrimitiveCount(vertices.size());
	case LINES:
		geometry->SetPrimitiveCount(vertices.size() / 2);
	case TRIANGLES:
		geometry->SetPrimitiveCount(vertices.size() / 3);
	default:
		break;
	}
	geometry->Update();
	MarkedDirty();
}

void CustomGeometry::SetMaterial(MaterialPtr material)
{
	geometry->SetMaterial(material);
}