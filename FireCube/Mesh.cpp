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
#include "Mesh.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "Renderer.h"				
#include "Application.h"
#include "tinyxml.h"
#include "ModelLoaders.h"

using namespace FireCube;
MaterialResource::MaterialResource()
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
Mesh::Mesh() : material(Material())
{
	
}
Mesh::~Mesh()
{

}
Object::Object()
{

}
ModelResource::ModelResource()
{

}
ModelResource::~ModelResource()
{	
	Logger::Write(string("Destroyed model with name:"));
	Logger::Write(name);
	Logger::Write(".\n");
}

bool ModelResource::Load(const string &filename)
{
	string::size_type d;
	d=filename.find_last_of(".");
	name=filename;
	ostringstream ss;
	ss<< "Created model with name:"<<filename<<endl;
	Logger::Write(ss.str());
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="3ds")
		{				
			M3dsLoader m3dsLoader(this);
			
			if (m3dsLoader.Load(filename))							
			{
				CalculateBoundingBox();
				return true;			
			}
			else
				return false;
		}
		if (ext=="dae")
		{				
			ColladaLoader colladaLoader(filename);

			if (colladaLoader.Load())
			{
				colladaLoader.GenerateModel(this);
				CalculateBoundingBox();
				UpdateBuffers();
				return true;
			}
			else
				return false;				
		}
	}
	return false;
}
void ModelResource::CalculateBoundingBox()
{
	for (vector<Object>::iterator i=object.begin();i!=object.end();i++)
	{
		for (vector<vec3>::iterator j=i->vertex.begin();j!=i->vertex.end();j++)
		{
			i->bbox.Expand(*j);
		}
	}
	for (vector<Object>::iterator i=object.begin();i!=object.end();i++)
	{
		bbox.Expand(i->bbox);
	}
}
BoundingBox ModelResource::GetBoundingBox()
{
	return bbox;
}
void ModelResource::CalculateNormals()
{	
	for (DWORD k=0;k<object.size();k++)
	{
		object[k].normal.resize(object[k].vertex.size());		
		for (DWORD n=0;n<object[k].normal.size();n++)
		{			
			object[k].normal[n]=vec3(0,0,0);		
		}
		for (DWORD f=0;f<object[k].face.size();f++)
		{
			vec3 v1=object[k].vertex[object[k].face[f].v[1]]-object[k].vertex[object[k].face[f].v[0]];
			vec3 v2=object[k].vertex[object[k].face[f].v[2]]-object[k].vertex[object[k].face[f].v[0]];
			vec3 n=Cross(v1,v2);			
			object[k].face[f].normal=n;
			object[k].face[f].normal.Normalize();
			/*for (unsigned int i=0;i<object[k].vertex.size();i++)					// Fix texture coordinate problem having certain vertices duplicated. SLOW.
			{
			if (object[k].vertex[i]==object[k].vertex[object[k].face[f].v[0]])
			{				
			object[k].normal[i]+=n;
			count[i]++;				
			}
			if (object[k].vertex[i]==object[k].vertex[object[k].face[f].v[1]])
			{				
			object[k].normal[i]+=n;
			count[i]++;
			}
			if (object[k].vertex[i]==object[k].vertex[object[k].face[f].v[2]])
			{				
			object[k].normal[i]+=n;
			count[i]++;
			}
			}*/
			object[k].normal[object[k].face[f].v[0]]+=n;			
			object[k].normal[object[k].face[f].v[1]]+=n;			
			object[k].normal[object[k].face[f].v[2]]+=n;			
		}		
		for (DWORD n=0;n<object[k].normal.size();n++)
		{			
			object[k].normal[n].Normalize();		
		}	
		object[k].normalBuffer=Buffer(new BufferResource);
		object[k].normalBuffer->Create();
		object[k].normalBuffer->LoadData(&object[k].normal[0],sizeof(vec3)*object[k].normal.size(),STATIC);
	}
}
void ModelResource::ApplyTransformation(mat4 &transform)
{
	for (DWORD k=0;k<object.size();k++)
	{
		for (DWORD i=0;i<object[k].vertex.size();i++)
		{
			object[k].vertex[i]=object[k].vertex[i]*transform;
		}
		object[k].vertexBuffer->LoadData(&object[k].vertex[0],sizeof(vec3)*object[k].vertex.size(),STATIC);
	}
	CalculateNormals();
}

Material ModelResource::GetMaterialByName(const string &name)
{
	for (DWORD i=0;i<material.size();i++)
	{
		if (material[i]->name==name)
			return material[i];
	}
	return Material();
}
Model ModelResource::Reduce()
{
	Model model(new ModelResource);
	for (unsigned int l=0;l<object.size();l++)
	{
		Object *curObject;
		model->object.push_back(Object());
		curObject=&(model->object[model->object.size()-1]);
		curObject->face=object[l].face;
		curObject->name=object[l].name;

		for (unsigned int i=0;i<curObject->face.size();i++)
		{
			for (unsigned int j=0;j<3;j++)
			{
				vec3 v=object[l].vertex[curObject->face[i].v[j]];
				bool found=false;
				unsigned int k;
				for (k=0;k<curObject->vertex.size();k++)
				{
					if ((curObject->vertex[k]-v).Length2()==0.0f)
					{
						found=true;
						break;
					}
				}
				if (found==false)
				{
					curObject->vertex.push_back(v);
					curObject->face[i].v[j]=curObject->vertex.size()-1;
				}
				else
				{
					curObject->face[i].v[j]=k;
				}
			}
		}
	}
	return model;
}
void ModelResource::SetProgram(const Program &program)
{	
	vector<Material>::iterator i=material.begin();
	for (;i!=material.end();i++)
	{				
		(*i)->program=program;
	}
}
void ModelResource::CreateHardNormals()
{
	vector<vec3> vertices;
	vector<vec2> diffuseUV;
	for (DWORD k=0;k<object.size();k++)
	{
		DWORD currentIndex=0;
		Object &obj=object[k];
		vertices.clear();
		diffuseUV.clear();
		vertices=obj.vertex;
		diffuseUV=obj.diffuseUV;
		obj.face.clear();
		obj.vertex.clear();
		obj.normal.clear();
		obj.diffuseUV.clear();

		for (DWORD j=0;j<obj.mesh.size();j++)
		{
			Mesh &ms=obj.mesh[j];
			for (DWORD i=0;i<ms.face.size();i++)
			{				
				vec3 v0=vertices[ms.face[i].v[0]];
				vec3 v1=vertices[ms.face[i].v[1]];
				vec3 v2=vertices[ms.face[i].v[2]];
				vec3 n=Cross(v1-v0,v2-v0).Normalize();
				obj.vertex.push_back(v0);
				obj.vertex.push_back(v1);
				obj.vertex.push_back(v2);
				obj.normal.push_back(n);
				obj.normal.push_back(n);
				obj.normal.push_back(n);
				if (diffuseUV.size()>0)
				{
					vec2 uv0(0,0),uv1(0,0),uv2(0,0);
					uv0=diffuseUV[ms.face[i].v[0]];
					uv1=diffuseUV[ms.face[i].v[1]];
					uv2=diffuseUV[ms.face[i].v[2]];

					obj.diffuseUV.push_back(uv0);
					obj.diffuseUV.push_back(uv1);
					obj.diffuseUV.push_back(uv2);
				}									
				ms.face[i].v[0]=currentIndex++;
				ms.face[i].v[1]=currentIndex++;
				ms.face[i].v[2]=currentIndex++;				
				obj.face.push_back(ms.face[i]);
			}
			vector<DWORD> tmp;
			tmp.resize(ms.face.size()*3);
			for (DWORD f=0;f<ms.face.size();f++)
			{
				tmp[f*3+0]=ms.face[f].v[0];
				tmp[f*3+1]=ms.face[f].v[1];
				tmp[f*3+2]=ms.face[f].v[2];
			}			
		}		
		if (obj.diffuseUV.size())
		{	
			obj.diffuseUVBuffer=Buffer(new BufferResource);
			obj.diffuseUVBuffer->Create();
			obj.diffuseUVBuffer->LoadData(&obj.diffuseUV[0],sizeof(vec2)*obj.diffuseUV.size(),STATIC);
		}		
	}
	UpdateBuffers();
}
void ModelResource::UpdateBuffers()
{
	for (DWORD k=0;k<object.size();k++)
	{	
		Object &obj=object[k];
		if (!obj.vertexBuffer)
		{
			obj.vertexBuffer=Buffer(new BufferResource);
			obj.vertexBuffer->Create();
		}
		if (!obj.vertexBuffer->LoadData(&obj.vertex[0],sizeof(vec3)*obj.vertex.size(),STATIC))
		{
			ostringstream oss;
			oss << "buffer id:"<<obj.vertexBuffer->id << " Couldn't upload vertex data" << endl;
			Logger::Write(oss.str());
		}
		if (obj.normal.size()!=0)
		{
			if (!obj.normalBuffer)
			{
				obj.normalBuffer=Buffer(new BufferResource);
				obj.normalBuffer->Create();
			}
			if (!obj.normalBuffer->LoadData(&obj.normal[0],sizeof(vec3)*obj.normal.size(),STATIC))
			{
				ostringstream oss;
				oss << "buffer id:"<<obj.vertexBuffer->id << " Couldn't upload vertex data" << endl;
				Logger::Write(oss.str());
			}
		}
		else
			obj.normalBuffer.reset();
		
		if (obj.diffuseUV.size()!=0)
		{
			if (!obj.diffuseUVBuffer)
			{
				obj.diffuseUVBuffer=Buffer(new BufferResource);
				obj.diffuseUVBuffer->Create();
			}
			if (!obj.diffuseUVBuffer->LoadData(&obj.diffuseUV[0],sizeof(vec2)*obj.diffuseUV.size(),STATIC))
			{
				ostringstream oss;
				oss << "buffer id:"<<obj.vertexBuffer->id << " Couldn't upload vertex data" << endl;
				Logger::Write(oss.str());
			}
		}
		else
			obj.diffuseUVBuffer.reset();

		

		for (DWORD m=0;m<obj.mesh.size();m++)
		{
			Mesh &mesh=obj.mesh[m];
			vector<DWORD> indices;
			for (DWORD i=0;i<mesh.face.size();i++)
			{
				indices.push_back(mesh.face[i].v[0]);
				indices.push_back(mesh.face[i].v[1]);
				indices.push_back(mesh.face[i].v[2]);
			}
			if (!mesh.indexBuffer)
			{
				mesh.indexBuffer=Buffer(new BufferResource);
				mesh.indexBuffer->Create();
			}
			if (!mesh.indexBuffer->LoadIndexData(&indices[0],indices.size(),STATIC))
			{
				ostringstream oss;
				oss << "buffer id:"<<obj.vertexBuffer->id << " Couldn't upload vertex data" << endl;
				Logger::Write(oss.str());
			}
		}
	}
}