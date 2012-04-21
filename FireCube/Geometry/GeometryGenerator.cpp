#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
using namespace std;
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Geometry/GeometryGenerator.h"
#include "Geometry/Material.h"
using namespace FireCube;

MaterialPtr CreateDefaultMaterial()
{
	MaterialPtr material(new Material);
	material->SetName("Default");
	material->SetAmbientColor(vec3(0.3f, 0.3f, 0.3f));
	material->SetDiffuseColor(vec3(0.7f, 0.7f, 0.7f));
	material->SetSpecularColor(vec3(0.2f, 0.2f, 0.2f));
	material->SetShininess(50.0f);
	return material;
}

GeometryPtr FIRECUBE_API GeometryGenerator::GenerateBox(const vec3 &size, MaterialPtr material)
{
	GeometryPtr ret(new Geometry);
	vec3 halfSize = size * 0.5f;

	// Front
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, halfSize.z));
	ret->GetNormals().push_back(vec3(0, 0, 1));
	ret->GetNormals().push_back(vec3(0, 0, 1));
	ret->GetNormals().push_back(vec3(0, 0, 1));
	ret->GetNormals().push_back(vec3(0, 0, 1));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(0, 0, 1);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(0, 0, 1);
	// Back
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, -halfSize.z));
	ret->GetNormals().push_back(vec3(0, 0, -1));
	ret->GetNormals().push_back(vec3(0, 0, -1));
	ret->GetNormals().push_back(vec3(0, 0, -1));
	ret->GetNormals().push_back(vec3(0, 0, -1));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(0, 0, -1);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(0, 0, -1);
	// Left
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, halfSize.z));
	ret->GetNormals().push_back(vec3(-1, 0, 0));
	ret->GetNormals().push_back(vec3(-1, 0, 0));
	ret->GetNormals().push_back(vec3(-1, 0, 0));
	ret->GetNormals().push_back(vec3(-1, 0, 0));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(-1, 0, 0);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(-1, 0, 0);
	// Right
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, -halfSize.z));
	ret->GetNormals().push_back(vec3(1, 0, 0));
	ret->GetNormals().push_back(vec3(1, 0, 0));
	ret->GetNormals().push_back(vec3(1, 0, 0));
	ret->GetNormals().push_back(vec3(1, 0, 0));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(1, 0, 0);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(1, 0, 0);
	// Top
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, halfSize.y, -halfSize.z));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(0, 1, 0);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(0, 1, 0);
	// Bottom
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, -halfSize.z));
	ret->GetVertices().push_back(vec3(-halfSize.x, -halfSize.y, halfSize.z));
	ret->GetVertices().push_back(vec3(halfSize.x, -halfSize.y, halfSize.z));
	ret->GetNormals().push_back(vec3(0, -1, 0));
	ret->GetNormals().push_back(vec3(0, -1, 0));
	ret->GetNormals().push_back(vec3(0, -1, 0));
	ret->GetNormals().push_back(vec3(0, -1, 0));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 4, ret->GetVertices().size() - 3, ret->GetVertices().size() - 2));
	ret->GetFaces().back().normal = vec3(0, -1, 0);
	ret->GetFaces().push_back(Face(ret->GetVertices().size() - 2, ret->GetVertices().size() - 3, ret->GetVertices().size() - 1));
	ret->GetFaces().back().normal = vec3(0, -1, 0);

	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(ret->GetFaces().size());
	ret->SetVertexCount(ret->GetFaces().size() * 3);
	ret->CopyFacesToIndexBuffer();

	ret->SetMaterial(material);
	ret->CalculateBoundingBox();
	ret->UpdateBuffers();
	return ret;
}
GeometryPtr FIRECUBE_API GeometryGenerator::GenerateBox(const vec3 &size)
{
	return GenerateBox(size, CreateDefaultMaterial());
}

GeometryPtr FIRECUBE_API GeometryGenerator::GenerateSphere (float radius, unsigned int rings, unsigned int columns, MaterialPtr material)
{
	GeometryPtr ret(new Geometry);
	vector<unsigned int> indices;

	ret->GetVertices().reserve((rings + 1) * (columns + 1));
	ret->GetNormals().reserve((rings + 1) * (columns + 1));
	ret->GetDiffuseUV().reserve((rings + 1) * (columns + 1));
	indices.reserve(2 * rings * (columns + 1));

	const float fDeltaRingAngle = (float)PI / rings;
	const float fDeltacolumnAngle = 2 * (float)PI / columns;

	unsigned int vertexIndex = 0;

	for(unsigned int ring = 0; ring < rings + 1 ; ring++)
	{
		const float r0 = std::sin(ring * fDeltaRingAngle);
		const float y0 = std::cos(ring * fDeltaRingAngle);

		for(unsigned column = 0; column < columns + 1 ; column++)
		{
			vec3 pos(-r0 * std::sin(column * fDeltacolumnAngle), y0, -r0 * std::cos(column * fDeltacolumnAngle));
			vec2 uv(column / (float)columns, ring / (float) rings);

			ret->GetVertices().push_back(pos * radius);
			ret->GetNormals().push_back(pos.Normalize());
			ret->GetDiffuseUV().push_back(uv);
			if(ring < rings)
			{
				indices.push_back(vertexIndex);
				indices.push_back(vertexIndex + columns + 1);
				++vertexIndex;
			}
		}
	}
	ret->GetFaces().push_back(Face(indices[0], indices[1], indices[2]));
	vec3 p = ret->GetVertices()[indices[1]] - ret->GetVertices()[indices[0]];
	vec3 q = ret->GetVertices()[indices[2]] - ret->GetVertices()[indices[0]]; 
	ret->GetFaces().back().normal = Cross(p, q).Normalize();
	for (unsigned int i = 3; i < indices.size(); i++)
	{
		if (i % 2 == 1)
		{
			Face f(indices[i - 1], indices[i - 2], indices[i]);
			ret->GetFaces().push_back(f);
			vec3 p = ret->GetVertices()[indices[i - 2]] - ret->GetVertices()[indices[i - 1]];
			vec3 q = ret->GetVertices()[indices[i]] - ret->GetVertices()[indices[i - 1]]; 
			ret->GetFaces().back().normal = Cross(p, q).Normalize();
		}
		else
		{
			Face f(indices[i - 2], indices[i - 1], indices[i]);
			ret->GetFaces().push_back(f);
			vec3 p = ret->GetVertices()[indices[i - 1]] - ret->GetVertices()[indices[i - 2]];
			vec3 q = ret->GetVertices()[indices[i]] - ret->GetVertices()[indices[i - 2]]; 
			ret->GetFaces().back().normal = Cross(p, q).Normalize();
		}
	}

	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(ret->GetFaces().size());
	ret->SetVertexCount(ret->GetFaces().size() * 3);
	ret->CopyFacesToIndexBuffer();

	ret->SetMaterial(material);
	ret->CalculateBoundingBox();
	ret->UpdateBuffers();
	return ret;
}
GeometryPtr FIRECUBE_API GeometryGenerator::GenerateSphere (float radius, unsigned int rings, unsigned int columns)
{
	return GenerateSphere(radius, rings, columns, CreateDefaultMaterial());
}
GeometryPtr FIRECUBE_API GeometryGenerator::GeneratePlane(const vec2 &size)
{
	return GeneratePlane(size, CreateDefaultMaterial());
}
GeometryPtr FIRECUBE_API GeometryGenerator::GeneratePlane(const vec2 &size, MaterialPtr material)
{
	GeometryPtr ret(new Geometry);
	vec2 halfSize = size * 0.5f;

	ret->GetVertices().push_back(vec3(-halfSize.x, 0, halfSize.y));
	ret->GetVertices().push_back(vec3(halfSize.x, 0, halfSize.y));
	ret->GetVertices().push_back(vec3(-halfSize.x, 0, -halfSize.y));
	ret->GetVertices().push_back(vec3(halfSize.x, 0, -halfSize.y));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetNormals().push_back(vec3(0, 1, 0));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 1.0f));
	ret->GetDiffuseUV().push_back(vec2(0.0f, 0.0f));
	ret->GetDiffuseUV().push_back(vec2(1.0f, 0.0f));
	ret->GetFaces().push_back(Face(0, 1, 2));
	ret->GetFaces().back().normal = vec3(0, 1, 0);
	ret->GetFaces().push_back(Face(2, 1, 3));
	ret->GetFaces().back().normal = vec3(0, 1, 0);

	ret->SetPrimitiveType(TRIANGLES);
	ret->SetPrimitiveCount(ret->GetFaces().size());
	ret->SetVertexCount(ret->GetFaces().size() * 3);
	ret->CopyFacesToIndexBuffer();

	ret->SetMaterial(material);
	ret->CalculateBoundingBox();
	ret->UpdateBuffers();
	return ret;
}



/*
GeometryPtr FIRECUBE_API GeometryGenerator::GenerateSphere (float radius, unsigned int rings, unsigned int columns, MaterialPtr material)
{
GeometryPtr ret(new Geometry);
vector<unsigned int> indices;

ret->GetVertices().reserve((rings + 1) * (columns + 1));
ret->GetNormals().reserve((rings + 1) * (columns + 1));
ret->GetDiffuseUV().reserve((rings + 1) * (columns + 1));
indices.reserve(6 * rings * (columns + 1));

const float fDeltaRingAngle = (float)PI / rings;
const float fDeltacolumnAngle = 2 * (float)PI / columns;

unsigned int vertexIndex = 0;

for(unsigned int ring = 0; ring < rings + 1 ; ring++)
{
const float r0 = std::sin(ring * fDeltaRingAngle);
const float y0 = std::cos(ring * fDeltaRingAngle);

for(unsigned column = 0; column < columns + 1 ; column++)
{
vec3 pos(-r0 * std::sin(column * fDeltacolumnAngle), y0, -r0 * std::cos(column * fDeltacolumnAngle));
vec2 uv(column / (float)columns, ring / (float) rings);

ret->GetVertices().push_back(pos * radius);
ret->GetNormals().push_back(pos.Normalize());
ret->GetDiffuseUV().push_back(uv);
if(ring != rings)
{				
indices.push_back(vertexIndex + columns + 1);
indices.push_back(vertexIndex);
indices.push_back(vertexIndex + columns);
indices.push_back(vertexIndex + columns + 1);
indices.push_back(vertexIndex + 1);
indices.push_back(vertexIndex);
++vertexIndex;
}
}
}
for (unsigned int i = 0; i < indices.size(); i +=3)
{
Face f(indices[i], indices[i + 1], indices[i + 2]);
ret->GetFaces().push_back(f);
vec3 p = ret->GetVertices()[indices[i + 1]] - ret->GetVertices()[indices[i]];
vec3 q = ret->GetVertices()[indices[i + 2]] - ret->GetVertices()[indices[i]]; 
ret->GetFaces().back().normal = Cross(p, q).Normalize();
}			

ret->SetPrimitiveType(TRIANGLES);
ret->SetPrimitiveCount(ret->GetFaces().size());
ret->SetVertexCount(ret->GetFaces().size() * 3);
ret->CopyFacesToIndexBuffer();

ret->SetMaterial(material);
ret->CalculateBoundingBox();
ret->UpdateBuffers();
return ret;
}
*/