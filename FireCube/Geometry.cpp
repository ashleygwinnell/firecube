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
#include "GLee.h"

#include "utils.h"
#include "Logger.h"
#include "MyMath.h"
#include "BoundingBox.h"
#include "Buffer.h"
#include "Geometry.h"

using namespace FireCube;
MaterialResource::MaterialResource() : shininess(128.0f)
{

}
MaterialResource::~MaterialResource()
{

}
string MaterialResource::GetName()
{
    return name;
}
void MaterialResource::SetName(const string &name)
{
    this->name = name;
}
vec4 MaterialResource::GetAmbientColor()
{
    return ambient;
}
void MaterialResource::SetAmbientColor(vec4 color)
{
    ambient = color;
}
vec4 MaterialResource::GetDiffuseColor()
{
    return diffuse;
}
void MaterialResource::SetDiffuseColor(vec4 color)
{
    diffuse = color;
}
vec4 MaterialResource::GetSpecularColor()
{
    return specular;
}
void MaterialResource::SetSpecularColor(vec4 color)
{
    specular = color;
}
float MaterialResource::GetShininess()
{
    return shininess;
}
void MaterialResource::SetShininess(float value)
{
    shininess = value;
}
Texture MaterialResource::GetDiffuseTexture()
{
    return diffuseTexture;
}
void MaterialResource::SetDiffuseTexture(Texture texture)
{
    diffuseTexture = texture;
}
Texture MaterialResource::GetNormalTexture()
{
    return normalTexture;
}
void MaterialResource::SetNormalTexture(Texture texture)
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
GeometryResource::~GeometryResource()
{
    Logger::Write(Logger::LOG_INFO, string("Destroyed geometry"));
}
void GeometryResource::CalculateNormals()
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
        /*for (unsigned int i=0;i<vertex.size();i++)					// Fix texture coordinate problem having certain vertices duplicated. SLOW.
        {
        if (vertex[i]==vertex[face[f].v[0]])
        {
        normal[i]+=n;
        count[i]++;
        }
        if (vertex[i]==vertex[face[f].v[1]])
        {
        normal[i]+=n;
        count[i]++;
        }
        if (vertex[i]==vertex[face[f].v[2]])
        {
        normal[i]+=n;
        count[i]++;
        }
        }*/
        normal[face[f].v[0]] += n;
        normal[face[f].v[1]] += n;
        normal[face[f].v[2]] += n;
    }
    for (unsigned int n = 0; n < normal.size(); n++)
    {
        normal[n].Normalize();
    }
    normalBuffer = Buffer(new BufferResource);
    normalBuffer->Create();
    normalBuffer->LoadData(&normal[0], sizeof(vec3)*normal.size(), STATIC);
}
void GeometryResource::CalculateTangents()
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
    tangentBuffer = Buffer(new BufferResource);
    tangentBuffer->Create();
    tangentBuffer->LoadData(&tangent[0], sizeof(vec3)*tangent.size(), STATIC);
    bitangentBuffer = Buffer(new BufferResource);
    bitangentBuffer->Create();
    bitangentBuffer->LoadData(&bitangent[0], sizeof(vec3)*bitangent.size(), STATIC);
}
void GeometryResource::CreateHardNormals()
{
    vector<vec3> originalVertices;
    vector<vec2> originalDiffuseUV;
    unsigned int currentIndex = 0;
    originalVertices = vertex;
    originalDiffuseUV = diffuseUV;
    face.clear();
    vertex.clear();
    normal.clear();
    diffuseUV.clear();

    for (unsigned int j = 0; j < surface.size(); j++)
    {
        Surface &surface = this->surface[j];
        for (unsigned int i = 0; i < surface.face.size(); i++)
        {
            vec3 v0 = originalVertices[surface.face[i].v[0]];
            vec3 v1 = originalVertices[surface.face[i].v[1]];
            vec3 v2 = originalVertices[surface.face[i].v[2]];
            vec3 n = Cross(v1 - v0, v2 - v0).Normalize();
            vertex.push_back(v0);
            vertex.push_back(v1);
            vertex.push_back(v2);
            normal.push_back(n);
            normal.push_back(n);
            normal.push_back(n);
            if (originalDiffuseUV.size() > 0)
            {
                vec2 uv0(0, 0), uv1(0, 0), uv2(0, 0);
                uv0 = originalDiffuseUV[surface.face[i].v[0]];
                uv1 = originalDiffuseUV[surface.face[i].v[1]];
                uv2 = originalDiffuseUV[surface.face[i].v[2]];

                diffuseUV.push_back(uv0);
                diffuseUV.push_back(uv1);
                diffuseUV.push_back(uv2);
            }
            surface.face[i].v[0] = currentIndex++;
            surface.face[i].v[1] = currentIndex++;
            surface.face[i].v[2] = currentIndex++;
            face.push_back(surface.face[i]);
        }
        vector<unsigned int> tmp;
        tmp.resize(surface.face.size() * 3);
        for (unsigned int f = 0; f < surface.face.size(); f++)
        {
            tmp[f * 3 + 0] = surface.face[f].v[0];
            tmp[f * 3 + 1] = surface.face[f].v[1];
            tmp[f * 3 + 2] = surface.face[f].v[2];
        }
    }
    if (diffuseUV.size())
    {
        diffuseUVBuffer = Buffer(new BufferResource);
        diffuseUVBuffer->Create();
        diffuseUVBuffer->LoadData(&diffuseUV[0], sizeof(vec2)*diffuseUV.size(), STATIC);
    }

    UpdateBuffers();
}
void GeometryResource::UpdateBuffers()
{
    if (!vertexBuffer)
    {
        vertexBuffer = Buffer(new BufferResource);
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
            normalBuffer = Buffer(new BufferResource);
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
        normalBuffer = Buffer();

    if (tangent.size() != 0)
    {
        if (!tangentBuffer)
        {
            tangentBuffer = Buffer(new BufferResource);
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
        tangentBuffer = Buffer();

    if (bitangent.size() != 0)
    {
        if (!bitangentBuffer)
        {
            bitangentBuffer = Buffer(new BufferResource);
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
        bitangentBuffer = Buffer();

    if (diffuseUV.size() != 0)
    {
        if (!diffuseUVBuffer)
        {
            diffuseUVBuffer = Buffer(new BufferResource);
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
        diffuseUVBuffer = Buffer();



    for (unsigned int m = 0; m < surface.size(); m++)
    {
        Surface &surface = this->surface[m];
        vector<unsigned int> indices;
        for (unsigned int i = 0; i < surface.face.size(); i++)
        {
            indices.push_back(surface.face[i].v[0]);
            indices.push_back(surface.face[i].v[1]);
            indices.push_back(surface.face[i].v[2]);
        }
        if (!surface.indexBuffer)
        {
            surface.indexBuffer = Buffer(new BufferResource);
            surface.indexBuffer->Create();
        }
        if (!surface.indexBuffer->LoadIndexData(&indices[0], indices.size(), STATIC))
        {
            ostringstream oss;
            oss << "buffer id:" << surface.indexBuffer->GetId() << " Couldn't upload vertex data";
            Logger::Write(Logger::LOG_ERROR, oss.str());
        }
    }
}
Geometry GeometryResource::Reduce()
{
    Geometry geometry(new GeometryResource);
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
Material GeometryResource::GetMaterialByName(const string &name)
{
    for (unsigned int i = 0; i < material.size(); i++)
    {
        if (material[i]->GetName() == name)
            return material[i];
    }
    return Material();
}
void GeometryResource::ApplyTransformation(mat4 &transform)
{
    for (unsigned int i = 0; i < vertex.size(); i++)
    {
        vertex[i] = vertex[i] * transform;
    }
    vertexBuffer->LoadData(&vertex[0], sizeof(vec3)*vertex.size(), STATIC);

    CalculateNormals();
}
BoundingBox GeometryResource::GetBoundingBox()
{
    return bbox;
}
void GeometryResource::CalculateBoundingBox()
{
    for (vector<vec3>::iterator j = vertex.begin(); j != vertex.end(); j++)
    {
        bbox.Expand(*j);
    }
}
vector<vec3> &GeometryResource::GetVertices()
{
    return vertex;
}
vector<vec3> &GeometryResource::GetNormals()
{
    return normal;
}
vector<vec3> &GeometryResource::GetTangents()
{
    return tangent;
}
vector<vec3> &GeometryResource::GetBitangents()
{
    return bitangent;
}
vector<Face> &GeometryResource::GetFaces()
{
    return face;
}
vector<vec2> &GeometryResource::GetDiffuseUV()
{
    return diffuseUV;
}
vector<Surface> &GeometryResource::GetSurfaces()
{
    return surface;
}
vector<Material> &GeometryResource::GetMaterials()
{
    return material;
}