#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/GLee.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Buffer.h"
#include "Geometry/Geometry.h"

using namespace FireCube;
Material::Material() : shininess(128.0f)
{

}
Material::~Material()
{

}
string Material::GetName() const
{
    return name;
}
void Material::SetName(const string &name)
{
    this->name = name;
}
vec4 Material::GetAmbientColor() const
{
    return ambient;
}
void Material::SetAmbientColor(const vec4 &color)
{
    ambient = color;
}
vec4 Material::GetDiffuseColor() const
{
    return diffuse;
}
void Material::SetDiffuseColor(const vec4 &color)
{
    diffuse = color;
}
vec4 Material::GetSpecularColor() const
{
    return specular;
}
void Material::SetSpecularColor(const vec4 &color)
{
    specular = color;
}
float Material::GetShininess() const
{
    return shininess;
}
void Material::SetShininess(float value)
{
    shininess = value;
}
TexturePtr Material::GetDiffuseTexture()
{
    return diffuseTexture;
}
void Material::SetDiffuseTexture(TexturePtr texture)
{
    diffuseTexture = texture;
}
TexturePtr Material::GetNormalTexture()
{
    return normalTexture;
}
void Material::SetNormalTexture(TexturePtr texture)
{
    normalTexture = texture;
}

Face::Face()
{

}
Face::Face(unsigned int v0, unsigned int v1, unsigned int v2)
{
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
}
Face::~Face()
{

}
Geometry::~Geometry()
{
    Logger::Write(Logger::LOG_INFO, string("Destroyed geometry"));
}
void Geometry::CalculateNormals()
{
    normal.resize(vertex.size());
    for (unsigned int n = 0; n < normal.size(); n++)
    {
        normal[n] = vec3(0, 0, 0);
    }
    for (unsigned int f = 0; f < face.size(); f++)
    {
        vec3 v1 = vertex[face[f].v[1]] - vertex[face[f].v[0]];
        vec3 v2 = vertex[face[f].v[2]] - vertex[face[f].v[0]];
        vec3 n = Cross(v1, v2);
        face[f].normal = n;
        face[f].normal.Normalize();       
        normal[face[f].v[0]] += n;
        normal[face[f].v[1]] += n;
        normal[face[f].v[2]] += n;
    }
    for (unsigned int n = 0; n < normal.size(); n++)
    {
        normal[n].Normalize();
    }
    normalBuffer = BufferPtr(new Buffer);
    normalBuffer->Create();
    normalBuffer->LoadData(&normal[0], sizeof(vec3)*normal.size(), STATIC);
}
void Geometry::CalculateTangents()
{
    if ((normal.size() == 0) || (diffuseUV.size() == 0))
        return;
    vector<vec3> tan(vertex.size() * 2);
    for (unsigned int i = 0; i < tan.size(); i++)
        tan[i].Set(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < face.size(); i++)
    {
        Face &face = this->face[i];

        vec3 v0 = vertex[face.v[0]];
        vec3 v1 = vertex[face.v[1]];
        vec3 v2 = vertex[face.v[2]];

        vec2 uv0 = diffuseUV[face.v[0]];
        vec2 uv1 = diffuseUV[face.v[1]];
        vec2 uv2 = diffuseUV[face.v[2]];

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

        tan[face.v[0]] += sdir;
        tan[face.v[1]] += sdir;
        tan[face.v[2]] += sdir;

        tan[vertex.size() + face.v[0]] += tdir;
        tan[vertex.size() + face.v[1]] += tdir;
        tan[vertex.size() + face.v[2]] += tdir;
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
    tangentBuffer = BufferPtr(new Buffer);
    tangentBuffer->Create();
    tangentBuffer->LoadData(&tangent[0], sizeof(vec3)*tangent.size(), STATIC);
    bitangentBuffer = BufferPtr(new Buffer);
    bitangentBuffer->Create();
    bitangentBuffer->LoadData(&bitangent[0], sizeof(vec3)*bitangent.size(), STATIC);
}
void Geometry::CreateHardNormals()
{
    vector<vec3> originalVertices = vertex;
    vector<vec2> originalDiffuseUV = diffuseUV;
	vector<Face> originalFaces = face;
	vector<vec3> originalTangents = tangent;
	vector<vec3> originalBitangents = bitangent;
    unsigned int currentIndex = 0;
    face.clear();
    vertex.clear();
    normal.clear();
    diffuseUV.clear();
	tangent.clear();
	bitangent.clear();
	for (unsigned int i = 0; i < originalFaces.size(); i++)
	{
		vec3 v0 = originalVertices[originalFaces[i].v[0]];
		vec3 v1 = originalVertices[originalFaces[i].v[1]];
		vec3 v2 = originalVertices[originalFaces[i].v[2]];
		vec3 n = Cross(v1 - v0, v2 - v0).Normalize();
		vertex.push_back(v0);
		vertex.push_back(v1);
		vertex.push_back(v2);
		normal.push_back(n);
		normal.push_back(n);
		normal.push_back(n);
		if (originalDiffuseUV.size() > 0)
		{
			vec2 uv0, uv1, uv2;
			uv0 = originalDiffuseUV[originalFaces[i].v[0]];
			uv1 = originalDiffuseUV[originalFaces[i].v[1]];
			uv2 = originalDiffuseUV[originalFaces[i].v[2]];

			diffuseUV.push_back(uv0);
			diffuseUV.push_back(uv1);
			diffuseUV.push_back(uv2);
		}
		if (originalTangents.size() > 0)
		{
			vec3 t0, t1, t2;
			t0 = originalTangents[originalFaces[i].v[0]];
			t1 = originalTangents[originalFaces[i].v[1]];
			t2 = originalTangents[originalFaces[i].v[2]];

			tangent.push_back(t0);
			tangent.push_back(t1);
			tangent.push_back(t2);
		}
		if (originalBitangents.size() > 0)
		{
			vec3 t0, t1, t2;
			t0 = originalBitangents[originalFaces[i].v[0]];
			t1 = originalBitangents[originalFaces[i].v[1]];
			t2 = originalBitangents[originalFaces[i].v[2]];

			bitangent.push_back(t0);
			bitangent.push_back(t1);
			bitangent.push_back(t2);
		}
		Face f(currentIndex, currentIndex + 1, currentIndex + 2);
		f.normal = n;
		currentIndex+=3;
		face.push_back(f);
	}
	CopyFacesToIndexBuffer();
	CalculateBoundingBox();
    UpdateBuffers();
}
void Geometry::UpdateBuffers()
{
    if (!vertexBuffer)
    {
        vertexBuffer = BufferPtr(new Buffer);
        vertexBuffer->Create();
    }
    if (!vertexBuffer->LoadData(&vertex[0], sizeof(vec3)*vertex.size(), STATIC))
    {
        ostringstream oss;
        oss << "buffer id:" << vertexBuffer->GetId() << " Couldn't upload vertex data";
        Logger::Write(Logger::LOG_ERROR, oss.str());
    }
    if (normal.size() != 0)
    {
        if (!normalBuffer)
        {
            normalBuffer = BufferPtr(new Buffer);
            normalBuffer->Create();
        }
        if (!normalBuffer->LoadData(&normal[0], sizeof(vec3)*normal.size(), STATIC))
        {
            ostringstream oss;
            oss << "buffer id:" << normalBuffer->GetId() << " Couldn't upload vertex data";
            Logger::Write(Logger::LOG_ERROR, oss.str());
        }
    }
    else
        normalBuffer = BufferPtr();

    if (tangent.size() != 0)
    {
        if (!tangentBuffer)
        {
            tangentBuffer = BufferPtr(new Buffer);
            tangentBuffer->Create();
        }
        if (!tangentBuffer->LoadData(&tangent[0], sizeof(vec3)*tangent.size(), STATIC))
        {
            ostringstream oss;
            oss << "buffer id:" << tangentBuffer->GetId() << " Couldn't upload vertex data";
            Logger::Write(Logger::LOG_ERROR, oss.str());
        }
    }
    else
        tangentBuffer = BufferPtr();

    if (bitangent.size() != 0)
    {
        if (!bitangentBuffer)
        {
            bitangentBuffer = BufferPtr(new Buffer);
            bitangentBuffer->Create();
        }
        if (!bitangentBuffer->LoadData(&bitangent[0], sizeof(vec3)*bitangent.size(), STATIC))
        {
            ostringstream oss;
            oss << "buffer id:" << bitangentBuffer->GetId() << " Couldn't upload vertex data";
            Logger::Write(Logger::LOG_ERROR, oss.str());
        }
    }
    else
        bitangentBuffer = BufferPtr();

    if (diffuseUV.size() != 0)
    {
        if (!diffuseUVBuffer)
        {
            diffuseUVBuffer = BufferPtr(new Buffer);
            diffuseUVBuffer->Create();
        }
        if (!diffuseUVBuffer->LoadData(&diffuseUV[0], sizeof(vec2)*diffuseUV.size(), STATIC))
        {
            ostringstream oss;
            oss << "buffer id:" << diffuseUVBuffer->GetId() << " Couldn't upload vertex data";
            Logger::Write(Logger::LOG_ERROR, oss.str());
        }
    }
    else
        diffuseUVBuffer = BufferPtr();

	if (!indices.empty())
	{	
		if (!indexBuffer)
		{
			indexBuffer = BufferPtr(new Buffer);
			indexBuffer->Create();
		}
		if (!indexBuffer->LoadIndexData(&indices[0], indices.size(), STATIC))
		{
			ostringstream oss;
			oss << "buffer id:" << indexBuffer->GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
}
GeometryPtr Geometry::Reduce() const
{
    GeometryPtr geometry(new Geometry);
    geometry->face = face;

    for (unsigned int i = 0; i < geometry->face.size(); i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            vec3 v = vertex[geometry->face[i].v[j]];
            bool found = false;
            unsigned int k;
            for (k = 0; k < geometry->vertex.size(); k++)
            {
                if ((vertex[k] - v).Length2() == 0.0f)
                {
                    found = true;
                    break;
                }
            }
            if (found == false)
            {
                geometry->vertex.push_back(v);
                geometry->face[i].v[j] = geometry->vertex.size() - 1;
            }
            else
            {
                geometry->face[i].v[j] = k;
            }
        }
    }
    return geometry;
}
void Geometry::ApplyTransformation(const mat4 &transform)
{
    for (unsigned int i = 0; i < vertex.size(); i++)
    {
        vertex[i] = vertex[i] * transform;
    }
    vertexBuffer->LoadData(&vertex[0], sizeof(vec3)*vertex.size(), STATIC);

    CalculateNormals();
}
BoundingBox Geometry::GetBoundingBox() const
{	
    return bbox;
}
void Geometry::CalculateBoundingBox()
{
	bbox = BoundingBox();
    for (vector<vec3>::iterator j = vertex.begin(); j != vertex.end(); j++)
    {
        bbox.Expand(*j);
    }
}
vector<vec3> &Geometry::GetVertices()
{
    return vertex;
}
vector<vec3> &Geometry::GetNormals()
{
    return normal;
}
vector<vec3> &Geometry::GetTangents()
{
    return tangent;
}
vector<vec3> &Geometry::GetBitangents()
{
    return bitangent;
}
vector<Face> &Geometry::GetFaces()
{
    return face;
}
vector<vec2> &Geometry::GetDiffuseUV()
{
    return diffuseUV;
}
vector<unsigned int> &Geometry::GetIndices()
{
	return indices;
}
MaterialPtr Geometry::GetMaterial()
{
    return material;
}
void Geometry::SetMaterial(MaterialPtr material)
{
	this->material = material;
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
void Geometry::SetIndexCount(unsigned int indexCount)
{
	this->indexCount = indexCount;
}
unsigned int Geometry::GetIndexCount() const
{
	return indexCount;
}
void Geometry::CopyFacesToIndexBuffer()
{
	indices.resize(face.size() * 3);
	for (unsigned int i = 0; i < face.size(); i++)
	{
		indices[i * 3 + 0] = face[i].v[0];
		indices[i * 3 + 1] = face[i].v[1];
		indices[i * 3 + 2] = face[i].v[2];
	}
}