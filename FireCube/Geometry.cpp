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
#include "ShaderGenerator.h"
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
Face::Face()
{
	
}
Face::Face(DWORD v0,DWORD v1,DWORD v2)
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
	Logger::Write(string("Destroyed geometry.\n"));	
}
void GeometryResource::CalculateNormals()
{	
	normal.resize(vertex.size());		
	for (DWORD n=0;n<normal.size();n++)
	{			
		normal[n]=vec3(0,0,0);		
	}
	for (DWORD f=0;f<face.size();f++)
	{
		vec3 v1=vertex[face[f].v[1]]-vertex[face[f].v[0]];
		vec3 v2=vertex[face[f].v[2]]-vertex[face[f].v[0]];
		vec3 n=Cross(v1,v2);			
		face[f].normal=n;
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
		normal[face[f].v[0]]+=n;			
		normal[face[f].v[1]]+=n;			
		normal[face[f].v[2]]+=n;			
	}		
	for (DWORD n=0;n<normal.size();n++)
	{			
		normal[n].Normalize();		
	}	
	normalBuffer=Buffer(new BufferResource);
	normalBuffer->Create();
	normalBuffer->LoadData(&normal[0],sizeof(vec3)*normal.size(),STATIC);
}

void GeometryResource::CreateHardNormals()
{
	vector<vec3> originalVertices;
	vector<vec2> originalDiffuseUV;
	DWORD currentIndex=0;
	originalVertices=vertex;
	originalDiffuseUV=diffuseUV;
	face.clear();
	vertex.clear();
	normal.clear();
	diffuseUV.clear();

	for (DWORD j=0;j<surface.size();j++)
	{
		Surface &surface=this->surface[j];
		for (DWORD i=0;i<surface.face.size();i++)
		{				
			vec3 v0=originalVertices[surface.face[i].v[0]];
			vec3 v1=originalVertices[surface.face[i].v[1]];
			vec3 v2=originalVertices[surface.face[i].v[2]];
			vec3 n=Cross(v1-v0,v2-v0).Normalize();
			vertex.push_back(v0);
			vertex.push_back(v1);
			vertex.push_back(v2);
			normal.push_back(n);
			normal.push_back(n);
			normal.push_back(n);
			if (originalDiffuseUV.size()>0)
			{
				vec2 uv0(0,0),uv1(0,0),uv2(0,0);
				uv0=originalDiffuseUV[surface.face[i].v[0]];
				uv1=originalDiffuseUV[surface.face[i].v[1]];
				uv2=originalDiffuseUV[surface.face[i].v[2]];

				diffuseUV.push_back(uv0);
				diffuseUV.push_back(uv1);
				diffuseUV.push_back(uv2);
			}									
			surface.face[i].v[0]=currentIndex++;
			surface.face[i].v[1]=currentIndex++;
			surface.face[i].v[2]=currentIndex++;				
			face.push_back(surface.face[i]);
		}
		vector<DWORD> tmp;
		tmp.resize(surface.face.size()*3);
		for (DWORD f=0;f<surface.face.size();f++)
		{
			tmp[f*3+0]=surface.face[f].v[0];
			tmp[f*3+1]=surface.face[f].v[1];
			tmp[f*3+2]=surface.face[f].v[2];
		}			
	}		
	if (diffuseUV.size())
	{	
		diffuseUVBuffer=Buffer(new BufferResource);
		diffuseUVBuffer->Create();
		diffuseUVBuffer->LoadData(&diffuseUV[0],sizeof(vec2)*diffuseUV.size(),STATIC);
	}		

	UpdateBuffers();
}
void GeometryResource::UpdateBuffers()
{
	if (!vertexBuffer)
	{
		vertexBuffer=Buffer(new BufferResource);
		vertexBuffer->Create();
	}
	if (!vertexBuffer->LoadData(&vertex[0],sizeof(vec3)*vertex.size(),STATIC))
	{
		ostringstream oss;
		oss << "buffer id:"<<vertexBuffer->id << " Couldn't upload vertex data" << endl;
		Logger::Write(oss.str());
	}
	if (normal.size()!=0)
	{
		if (!normalBuffer)
		{
			normalBuffer=Buffer(new BufferResource);
			normalBuffer->Create();
		}
		if (!normalBuffer->LoadData(&normal[0],sizeof(vec3)*normal.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<vertexBuffer->id << " Couldn't upload vertex data" << endl;
			Logger::Write(oss.str());
		}
	}
	else
		normalBuffer.reset();

	if (diffuseUV.size()!=0)
	{
		if (!diffuseUVBuffer)
		{
			diffuseUVBuffer=Buffer(new BufferResource);
			diffuseUVBuffer->Create();
		}
		if (!diffuseUVBuffer->LoadData(&diffuseUV[0],sizeof(vec2)*diffuseUV.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<vertexBuffer->id << " Couldn't upload vertex data" << endl;
			Logger::Write(oss.str());
		}
	}
	else
		diffuseUVBuffer.reset();



	for (DWORD m=0;m<surface.size();m++)
	{
		Surface &surface=this->surface[m];
		vector<DWORD> indices;
		for (DWORD i=0;i<surface.face.size();i++)
		{
			indices.push_back(surface.face[i].v[0]);
			indices.push_back(surface.face[i].v[1]);
			indices.push_back(surface.face[i].v[2]);
		}
		if (!surface.indexBuffer)
		{
			surface.indexBuffer=Buffer(new BufferResource);
			surface.indexBuffer->Create();
		}
		if (!surface.indexBuffer->LoadIndexData(&indices[0],indices.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<surface.indexBuffer->id << " Couldn't upload vertex data" << endl;
			Logger::Write(oss.str());
		}
	}
}
Geometry GeometryResource::Reduce()
{
	Geometry geometry(new GeometryResource);
	geometry->face=face;	

	for (unsigned int i=0;i<geometry->face.size();i++)
	{
		for (unsigned int j=0;j<3;j++)
		{
			vec3 v=vertex[geometry->face[i].v[j]];
			bool found=false;
			unsigned int k;
			for (k=0;k<geometry->vertex.size();k++)
			{
				if ((vertex[k]-v).Length2()==0.0f)
				{
					found=true;
					break;
				}
			}
			if (found==false)
			{
				geometry->vertex.push_back(v);
				geometry->face[i].v[j]=geometry->vertex.size()-1;
			}
			else
			{
				geometry->face[i].v[j]=k;
			}
		}
	}
	return geometry;
}
Material GeometryResource::GetMaterialByName(const string &name)
{
	for (DWORD i=0;i<material.size();i++)
	{
		if (material[i]->name==name)
			return material[i];
	}
	return Material();
}
void GeometryResource::ApplyTransformation(mat4 &transform)
{	
	for (DWORD i=0;i<vertex.size();i++)
	{
		vertex[i]=vertex[i]*transform;
	}
	vertexBuffer->LoadData(&vertex[0],sizeof(vec3)*vertex.size(),STATIC);

	CalculateNormals();
}
BoundingBox GeometryResource::GetBoundingBox()
{
	return bbox;
}
void GeometryResource::CalculateBoundingBox()
{
	for (vector<vec3>::iterator j=vertex.begin();j!=vertex.end();j++)
	{
		bbox.Expand(*j);
	}		
}