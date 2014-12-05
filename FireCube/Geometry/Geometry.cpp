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
		glDeleteVertexArrays(1, &objectId);
	LOGINFO("Destroyed geometry");
}
/*
void Geometry::CalculateNormals()
{
	normal.resize(vertex.size());
	std::fill(normal.begin(), normal.end(), vec3(0, 0, 0));
	
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		// Calculate face normals
		vec3 v1 = vertex[indices[i + 1]] - vertex[indices[i + 0]];
		vec3 v2 = vertex[indices[i + 2]] - vertex[indices[i + 0]];
		vec3 n = Cross(v1, v2);				
		// Add this normal to the three vertex normals forming this face
		normal[indices[i + 0]] += n;
		normal[indices[i + 1]] += n;
		normal[indices[i + 2]] += n;
	}
	for (unsigned int n = 0; n < normal.size(); n++)
	{
		normal[n].Normalize();
	}	
}

void Geometry::CalculateTangents()
{
	if ((normal.size() == 0) || (diffuseUV.size() == 0))
		return;
	std::vector<vec3> tan(vertex.size() * 2);
	std::fill(tan.begin(), tan.end(), vec3(0, 0, 0));

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{		
		const vec3 &v0 = vertex[indices[i + 0]];
		const vec3 &v1 = vertex[indices[i + 1]];
		const vec3 &v2 = vertex[indices[i + 2]];

		const vec2 &uv0 = diffuseUV[indices[i + 0]];
		const vec2 &uv1 = diffuseUV[indices[i + 1]];
		const vec2 &uv2 = diffuseUV[indices[i + 2]];

		float x1 = v1.x - v0.x;
		float x2 = v2.x - v0.x;
		float y1 = v1.y - v0.y;
		float y2 = v2.y - v0.y;
		float z1 = v1.z - v0.z;
		float z2 = v2.z - v0.z;

		float s1 = uv1.x - uv0.x;
		float s2 = uv2.x - uv0.x;
		float t1 = uv1.y - uv0.y;
		float t2 = uv2.y - uv0.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				  (t2 * z1 - t1 * z2) * r);
		vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				  (s1 * z2 - s2 * z1) * r);

		tan[indices[i + 0]] += sdir;
		tan[indices[i + 1]] += sdir;
		tan[indices[i + 2]] += sdir;

		tan[vertex.size() + indices[i + 0]] += tdir;
		tan[vertex.size() + indices[i + 1]] += tdir;
		tan[vertex.size() + indices[i + 2]] += tdir;
	}
	tangent.resize(vertex.size());
	bitangent.resize(vertex.size());
	for (unsigned int i = 0; i < vertex.size(); i++)
	{
		vec3 n = normal[i];
		vec3 t1 = tan[i];
		vec3 t2 = tan[vertex.size() + i];

		tangent[i] = (t1 - n * Dot(n, t1)).Normalize();
		float w = (Dot(Cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
		bitangent[i] = (Cross(n, tangent[i]) * w).Normalize();
	}	
}

void Geometry::CreateHardNormals()
{
	// Makes this geometry have hard normals.
	// This is done by duplicating the vertices and setting each vertex normal to
	// be equal to the face normal it belongs to.
	if (primitiveType != TRIANGLES)
		return;
	std::vector<vec3> originalVertices = vertex;
	std::vector<unsigned int> originalIndices = indices;
	std::vector<vec2> originalDiffuseUV = diffuseUV;	
	std::vector<vec3> originalTangents = tangent;
	std::vector<vec3> originalBitangents = bitangent;
	unsigned int currentIndex = 0;	
	vertex.clear();
	normal.clear();
	diffuseUV.clear();
	tangent.clear();
	bitangent.clear();
	indices.clear();
	for (unsigned int i = 0; i < originalIndices.size(); i += 3)
	{
		vec3 v0 = originalVertices[originalIndices[i + 0]];
		vec3 v1 = originalVertices[originalIndices[i + 1]];
		vec3 v2 = originalVertices[originalIndices[i + 2]];
		vec3 n = Cross(v1 - v0, v2 - v0).Normalize();
		// Add three vertices for each face
		vertex.push_back(v0);
		vertex.push_back(v1);
		vertex.push_back(v2);
		// Duplicate the face normal three times
		normal.push_back(n);
		normal.push_back(n);
		normal.push_back(n);
		if (originalDiffuseUV.size() > 0)
		{
			vec2 uv0, uv1, uv2;
			uv0 = originalDiffuseUV[originalIndices[i + 0]];
			uv1 = originalDiffuseUV[originalIndices[i + 1]];
			uv2 = originalDiffuseUV[originalIndices[i + 2]];

			diffuseUV.push_back(uv0);
			diffuseUV.push_back(uv1);
			diffuseUV.push_back(uv2);
		}
		if (originalTangents.size() > 0)
		{
			vec3 t0, t1, t2;
			t0 = originalTangents[originalIndices[i + 0]];
			t1 = originalTangents[originalIndices[i + 1]];
			t2 = originalTangents[originalIndices[i + 2]];

			tangent.push_back(t0);
			tangent.push_back(t1);
			tangent.push_back(t2);
		}
		if (originalBitangents.size() > 0)
		{
			vec3 t0, t1, t2;
			t0 = originalBitangents[originalIndices[i + 0]];
			t1 = originalBitangents[originalIndices[i + 1]];
			t2 = originalBitangents[originalIndices[i + 2]];

			bitangent.push_back(t0);
			bitangent.push_back(t1);
			bitangent.push_back(t2);
		}
		indices.push_back(currentIndex + 0);
		indices.push_back(currentIndex + 1);
		indices.push_back(currentIndex + 2);
		currentIndex += 3;		
	}	
	SetVertexCount(indices.size());
	SetPrimitiveCount(indices.size() / 3);
	CalculateBoundingBox();
	UpdateBuffers();
}
*/
void Geometry::Update()
{
	if (objectId == 0)
		glGenVertexArrays(1, &objectId);

	glBindVertexArray(objectId);
	vertexBuffer->ApplyAttributes();	
	if (indexBuffer)
		indexBuffer->SetIndexStream();
	glBindVertexArray(0);
}

/*MaterialPtr Geometry::GetMaterial()
{
	return material;
}

void Geometry::SetMaterial(MaterialPtr material)
{
	this->material = material;
}*/

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