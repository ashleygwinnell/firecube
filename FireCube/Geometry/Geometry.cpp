#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Geometry/Geometry.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

Geometry::Geometry(Renderer *renderer) : GraphicsResource(renderer), primitiveCount(0), vertexBuffer(nullptr), indexBuffer(nullptr), geometryType(GeometryType::STATIC)
{

}

Geometry::~Geometry()
{
	delete vertexBuffer;
	delete indexBuffer;
	if (objectId)
	{
		glDeleteVertexArrays(1, &objectId);
	}
}

void Geometry::Update()
{
	if (objectId == 0)
	{
		glGenVertexArrays(1, &objectId);
	}

	glBindVertexArray(objectId);
	vertexBuffer->ApplyAttributes();	
	if (indexBuffer)
	{
		indexBuffer->SetIndexStream();
	}
	glBindVertexArray(0);
}

void Geometry::SetPrimitiveType(PrimitiveType primitiveType)
{
	this->primitiveType = primitiveType;
}

PrimitiveType Geometry::GetPrimitiveType() const
{
	return primitiveType;
}

void Geometry::SetPrimitiveCount(unsigned int primitiveCount)
{
	this->primitiveCount = primitiveCount;
}

unsigned int Geometry::GetPrimitiveCount() const
{
	return primitiveCount;
}

Geometry *Geometry::Clone()
{
	Geometry *ret = new Geometry(renderer);
	ret->primitiveCount = this->primitiveCount;
	ret->primitiveType = this->primitiveType;
	
	return ret;
}

void Geometry::SetVertexBuffer(VertexBuffer *vertexBuffer)
{
	this->vertexBuffer = vertexBuffer;
}

VertexBuffer *Geometry::GetVertexBuffer()
{
	return vertexBuffer;
}

void Geometry::SetIndexBuffer(IndexBuffer *indexBuffer)
{
	this->indexBuffer = indexBuffer;	
}

IndexBuffer *Geometry::GetIndexBuffer()
{
	return indexBuffer;
}

void Geometry::Render()
{
	if (primitiveCount > 0)
	{
		glBindVertexArray(objectId);

		if (indexBuffer && indexBuffer->IsValid())
			renderer->RenderIndexStream(primitiveType, indexBuffer->GetIndicesCount());
		else
			renderer->RenderStream(primitiveType, vertexBuffer->GetVertexCount());
		renderer->IncreamentNumberOfPrimitivesRendered(primitiveCount);

		glBindVertexArray(0);
	}
}

bool Geometry::IntersectRay(const Ray &ray, float &distance, vec3 &normal) const
{
	bool found = false;
	if (vertexBuffer && vertexBuffer->GetShadowData().empty() == false)
	{
		const char *vertexData = &vertexBuffer->GetShadowData()[0];
		if (indexBuffer && indexBuffer->GetShadowData().empty() == false)
		{
			const char *indexData = &indexBuffer->GetShadowData()[0];
			if (ray.IntersectMesh(vertexData, vertexBuffer->GetVertexSize(), indexData, indexBuffer->GetIndicesCount(), distance, normal))
			{
				found = true;
			}
		}
		else
		{
			if (ray.IntersectMesh(vertexData, vertexBuffer->GetVertexCount(), vertexBuffer->GetVertexSize(), distance, normal))
			{
				found = true;
			}
		}
	}
	return found;
}

void Geometry::SetGeometryType(GeometryType geometryType)
{
	this->geometryType = geometryType;
}

GeometryType Geometry::GetGeometryType() const
{
	return geometryType;
}