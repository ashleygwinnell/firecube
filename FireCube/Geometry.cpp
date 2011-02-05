#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include <GL/GLU.h>
#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Application.h"
#include "tinyxml.h"
#include "Light.h"
#include "Node.h"
#include "ModelLoaders.h"

using namespace FireCube;
MaterialResource::MaterialResource() : shininess(128.0f)
{
	
}
MaterialResource::~MaterialResource()
{

}
void Material::Create()
{
	resource=boost::shared_ptr<MaterialResource>(new MaterialResource);
}
string Material::GetName()
{
	return resource->name;
}
void Material::SetName(const string &name)
{
	resource->name=name;
}
vec4 Material::GetAmbientColor()
{
	return resource->ambient;
}
void Material::SetAmbientColor(vec4 color)
{
	resource->ambient=color;
}
vec4 Material::GetDiffuseColor()
{
	return resource->diffuse;
}
void Material::SetDiffuseColor(vec4 color)
{
	resource->diffuse=color;
}
vec4 Material::GetSpecularColor()
{
	return resource->specular;
}
void Material::SetSpecularColor(vec4 color)
{
	resource->specular=color;
}
float Material::GetShininess()
{
	return resource->shininess;
}
void Material::SetShininess(float value)
{
	resource->shininess=value;
}
Texture Material::GetDiffuseTexture()
{
	return resource->diffuseTexture;
}
void Material::SetDiffuseTexture(Texture texture)
{
	resource->diffuseTexture=texture;
}
Texture Material::GetNormalTexture()
{
	return resource->normalTexture;
}
void Material::SetNormalTexture(Texture texture)
{
	resource->normalTexture=texture;
}
Material::operator bool () const
{
	return resource;
}
bool Material::operator== (const Material &material) const
{
	return resource==material.resource;
}
Face::Face()
{
	
}
Face::Face(unsigned int v0,unsigned int v1,unsigned int v2)
{
	v[0]=v0;
	v[1]=v1;
	v[2]=v2;
}
Face::~Face()
{

}
GeometryResource::~GeometryResource()
{	
	Logger::Write(Logger::LOG_INFO, string("Destroyed geometry"));
}
void Geometry::Create()
{
	resource=boost::shared_ptr<GeometryResource>(new GeometryResource);
}
void Geometry::CalculateNormals()
{	
	resource->normal.resize(resource->vertex.size());		
	for (unsigned int n=0;n<resource->normal.size();n++)
	{			
		resource->normal[n]=vec3(0,0,0);		
	}
	for (unsigned int f=0;f<resource->face.size();f++)
	{
		vec3 v1=resource->vertex[resource->face[f].v[1]]-resource->vertex[resource->face[f].v[0]];
		vec3 v2=resource->vertex[resource->face[f].v[2]]-resource->vertex[resource->face[f].v[0]];
		vec3 n=Cross(v1,v2);			
		resource->face[f].normal=n;
		resource->face[f].normal.Normalize();
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
		resource->normal[resource->face[f].v[0]]+=n;			
		resource->normal[resource->face[f].v[1]]+=n;			
		resource->normal[resource->face[f].v[2]]+=n;			
	}		
	for (unsigned int n=0;n<resource->normal.size();n++)
	{			
		resource->normal[n].Normalize();		
	}		
	resource->normalBuffer.Create();
	resource->normalBuffer.LoadData(&resource->normal[0],sizeof(vec3)*resource->normal.size(),STATIC);
}
void Geometry::CalculateTangents()
{
	if ((resource->normal.size() == 0) || (resource->diffuseUV.size() == 0))
		return;
	vector<vec3> tan(resource->vertex.size()*2);
	for (unsigned int i=0;i<tan.size();i++)
		tan[i].Set(0.0f,0.0f,0.0f);

	for (unsigned int i=0;i<resource->face.size();i++)
	{
		Face &face=resource->face[i];
		
		vec3 v0=resource->vertex[face.v[0]];
		vec3 v1=resource->vertex[face.v[1]];
		vec3 v2=resource->vertex[face.v[2]];

		vec2 uv0=resource->diffuseUV[face.v[0]];
		vec2 uv1=resource->diffuseUV[face.v[1]];
		vec2 uv2=resource->diffuseUV[face.v[2]];

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

		tan[resource->vertex.size()+face.v[0]] += tdir;
		tan[resource->vertex.size()+face.v[1]] += tdir;
		tan[resource->vertex.size()+face.v[2]] += tdir;
	}
	resource->tangent.resize(resource->vertex.size());
	resource->bitangent.resize(resource->vertex.size());
	for (unsigned int i=0;i<resource->vertex.size();i++)
	{
		vec3 n=resource->normal[i];
		vec3 t1=tan[i];
		vec3 t2=tan[resource->vertex.size()+i];

		resource->tangent[i]=(t1 - n * Dot(n, t1)).Normalize();
		float w=(Dot(Cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
		resource->bitangent[i]=(Cross(n,resource->tangent[i])*w).Normalize();
	}
	resource->tangentBuffer.Create();
	resource->tangentBuffer.LoadData(&resource->tangent[0],sizeof(vec3)*resource->tangent.size(),STATIC);
	resource->bitangentBuffer.Create();
	resource->bitangentBuffer.LoadData(&resource->bitangent[0],sizeof(vec3)*resource->bitangent.size(),STATIC);
}
void Geometry::CreateHardNormals()
{
	vector<vec3> originalVertices;
	vector<vec2> originalDiffuseUV;
	unsigned int currentIndex=0;
	originalVertices=resource->vertex;
	originalDiffuseUV=resource->diffuseUV;
	resource->face.clear();
	resource->vertex.clear();
	resource->normal.clear();
	resource->diffuseUV.clear();

	for (unsigned int j=0;j<resource->surface.size();j++)
	{
		Surface &surface=this->resource->surface[j];
		for (unsigned int i=0;i<surface.face.size();i++)
		{				
			vec3 v0=originalVertices[surface.face[i].v[0]];
			vec3 v1=originalVertices[surface.face[i].v[1]];
			vec3 v2=originalVertices[surface.face[i].v[2]];
			vec3 n=Cross(v1-v0,v2-v0).Normalize();
			resource->vertex.push_back(v0);
			resource->vertex.push_back(v1);
			resource->vertex.push_back(v2);
			resource->normal.push_back(n);
			resource->normal.push_back(n);
			resource->normal.push_back(n);
			if (originalDiffuseUV.size()>0)
			{
				vec2 uv0(0,0),uv1(0,0),uv2(0,0);
				uv0=originalDiffuseUV[surface.face[i].v[0]];
				uv1=originalDiffuseUV[surface.face[i].v[1]];
				uv2=originalDiffuseUV[surface.face[i].v[2]];

				resource->diffuseUV.push_back(uv0);
				resource->diffuseUV.push_back(uv1);
				resource->diffuseUV.push_back(uv2);
			}									
			surface.face[i].v[0]=currentIndex++;
			surface.face[i].v[1]=currentIndex++;
			surface.face[i].v[2]=currentIndex++;				
			resource->face.push_back(surface.face[i]);
		}
		vector<unsigned int> tmp;
		tmp.resize(surface.face.size()*3);
		for (unsigned int f=0;f<surface.face.size();f++)
		{
			tmp[f*3+0]=surface.face[f].v[0];
			tmp[f*3+1]=surface.face[f].v[1];
			tmp[f*3+2]=surface.face[f].v[2];
		}			
	}		
	if (resource->diffuseUV.size())
	{			
		resource->diffuseUVBuffer.Create();
		resource->diffuseUVBuffer.LoadData(&resource->diffuseUV[0],sizeof(vec2)*resource->diffuseUV.size(),STATIC);
	}		

	UpdateBuffers();
}
void Geometry::UpdateBuffers()
{
	if (!resource->vertexBuffer)
	{		
		resource->vertexBuffer.Create();
	}
	if (!resource->vertexBuffer.LoadData(&resource->vertex[0],sizeof(vec3)*resource->vertex.size(),STATIC))
	{
		ostringstream oss;
		oss << "buffer id:"<<resource->vertexBuffer.GetId() << " Couldn't upload vertex data";
		Logger::Write(Logger::LOG_ERROR, oss.str());
	}
	if (resource->normal.size()!=0)
	{
		if (!resource->normalBuffer)
		{			
			resource->normalBuffer.Create();
		}
		if (!resource->normalBuffer.LoadData(&resource->normal[0],sizeof(vec3)*resource->normal.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<resource->normalBuffer.GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
	else
		resource->normalBuffer.Destroy();

	if (resource->tangent.size()!=0)
	{
		if (!resource->tangentBuffer)
		{			
			resource->tangentBuffer.Create();
		}
		if (!resource->tangentBuffer.LoadData(&resource->tangent[0],sizeof(vec3)*resource->tangent.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<resource->tangentBuffer.GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
	else
		resource->tangentBuffer.Destroy();

	if (resource->bitangent.size()!=0)
	{
		if (!resource->bitangentBuffer)
		{			
			resource->bitangentBuffer.Create();
		}
		if (!resource->bitangentBuffer.LoadData(&resource->bitangent[0],sizeof(vec3)*resource->bitangent.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<resource->bitangentBuffer.GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
	else
		resource->bitangentBuffer.Destroy();

	if (resource->diffuseUV.size()!=0)
	{
		if (!resource->diffuseUVBuffer)
		{			
			resource->diffuseUVBuffer.Create();
		}
		if (!resource->diffuseUVBuffer.LoadData(&resource->diffuseUV[0],sizeof(vec2)*resource->diffuseUV.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<resource->diffuseUVBuffer.GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
	else
		resource->diffuseUVBuffer.Destroy();



	for (unsigned int m=0;m<resource->surface.size();m++)
	{
		Surface &surface=this->resource->surface[m];
		vector<unsigned int> indices;
		for (unsigned int i=0;i<surface.face.size();i++)
		{
			indices.push_back(surface.face[i].v[0]);
			indices.push_back(surface.face[i].v[1]);
			indices.push_back(surface.face[i].v[2]);
		}
		if (!surface.indexBuffer)
		{			
			surface.indexBuffer.Create();
		}
		if (!surface.indexBuffer.LoadIndexData(&indices[0],indices.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<surface.indexBuffer.GetId() << " Couldn't upload vertex data";
			Logger::Write(Logger::LOG_ERROR, oss.str());
		}
	}
}
Geometry Geometry::Reduce()
{
	Geometry geometry;
	geometry.Create();
	geometry.resource->face=resource->face;	

	for (unsigned int i=0;i<geometry.resource->face.size();i++)
	{
		for (unsigned int j=0;j<3;j++)
		{
			vec3 v=resource->vertex[geometry.resource->face[i].v[j]];
			bool found=false;
			unsigned int k;
			for (k=0;k<geometry.resource->vertex.size();k++)
			{
				if ((resource->vertex[k]-v).Length2()==0.0f)
				{
					found=true;
					break;
				}
			}
			if (found==false)
			{
				geometry.resource->vertex.push_back(v);
				geometry.resource->face[i].v[j]=geometry.resource->vertex.size()-1;
			}
			else
			{
				geometry.resource->face[i].v[j]=k;
			}
		}
	}
	return geometry;
}
Material Geometry::GetMaterialByName(const string &name)
{
	for (unsigned int i=0;i<resource->material.size();i++)
	{
		if (resource->material[i].GetName()==name)
			return resource->material[i];
	}
	return Material();
}
void Geometry::ApplyTransformation(mat4 &transform)
{	
	for (unsigned int i=0;i<resource->vertex.size();i++)
	{
		resource->vertex[i]=resource->vertex[i]*transform;
	}
	resource->vertexBuffer.LoadData(&resource->vertex[0],sizeof(vec3)*resource->vertex.size(),STATIC);

	CalculateNormals();
}
BoundingBox Geometry::GetBoundingBox()
{
	return resource->bbox;
}
void Geometry::CalculateBoundingBox()
{
	for (vector<vec3>::iterator j=resource->vertex.begin();j!=resource->vertex.end();j++)
	{
		resource->bbox.Expand(*j);
	}		
}
vector<vec3> &Geometry::GetVertices()
{
	return resource->vertex;
}
vector<vec3> &Geometry::GetNormals()
{
	return resource->normal;
}
vector<vec3> &Geometry::GetTangents()
{
	return resource->tangent;
}
vector<vec3> &Geometry::GetBitangents()
{
	return resource->bitangent;
}
vector<Face> &Geometry::GetFaces()
{
	return resource->face;
}
vector<vec2> &Geometry::GetDiffuseUV()
{
	return resource->diffuseUV;
}
vector<Surface> &Geometry::GetSurfaces()
{
	return resource->surface;
}
vector<Material> &Geometry::GetMaterials()
{
	return resource->material;
}
Geometry::operator bool () const
{
	return resource;
}
bool Geometry::operator== (const Geometry &geometry) const
{
	return resource==geometry.resource;
}
