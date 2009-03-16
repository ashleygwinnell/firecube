#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include <GL/GLU.h>
#include "FireCube.h"
using namespace FireCube;
#include <dae.h>
#include <dom/domCOLLADA.h>
#include "ModelLoaders.h"
MaterialResource::MaterialResource()
{
	
}
MaterialResource::~MaterialResource()
{

}
Face::Face()
{
	
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
Object::Object() : uv(MAX_TEXTURES),uvBuffer(MAX_TEXTURES)
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
				return true;			
			else
				return false;
		}
		if (ext=="dae")
		{				
			ColladaLoader colladaLoader(this);

			if (colladaLoader.Load(filename))							
				return true;			
			else
				return false;
		}
	}
	return false;
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
	vector<vector<vec2>> uvs(8);
	for (DWORD k=0;k<object.size();k++)
	{
		DWORD currentIndex=0;
		Object &obj=object[k];
		vertices.clear();
		uvs.clear();
		vertices=obj.vertex;
		uvs=obj.uv;
		obj.face.clear();
		obj.vertex.clear();
		obj.normal.clear();
		for (DWORD t=0;t<MAX_TEXTURES;t++)		
			obj.uv[t].clear();

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
				for (DWORD t=0;t<MAX_TEXTURES;t++)
				{									
					if (uvs[t].size()>0)
					{
						vec2 uv0(0,0),uv1(0,0),uv2(0,0);
						uv0=uvs[t][ms.face[i].v[0]];
						uv1=uvs[t][ms.face[i].v[1]];
						uv2=uvs[t][ms.face[i].v[2]];

						obj.uv[t].push_back(uv0);
						obj.uv[t].push_back(uv1);
						obj.uv[t].push_back(uv2);
					}					
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
			ms.indexBuffer->LoadIndexData(&tmp[0],tmp.size(),STATIC);
		}
		obj.normalBuffer->LoadData(&obj.normal[0],sizeof(vec3)*obj.normal.size(),STATIC);
		for (DWORD t=0;t<MAX_TEXTURES;t++)
		{		
			if (obj.uv[t].size())
			{	
				obj.uvBuffer[t]=Buffer(new BufferResource);
				obj.uvBuffer[t]->Create();
				obj.uvBuffer[t]->LoadData(&obj.uv[t][0],sizeof(vec2)*obj.uv[t].size(),STATIC);
			}
		}
		obj.vertexBuffer->LoadData(&obj.vertex[0],sizeof(vec3)*obj.vertex.size(),STATIC);
	}
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
		
		for (DWORD t=0;t<MAX_TEXTURES;t++)
		{		
			if (obj.uv[t].size()!=0)
			{
				if (!obj.uvBuffer[t])
				{
					obj.uvBuffer[t]=Buffer(new BufferResource);
					obj.uvBuffer[t]->Create();
				}
				if (!obj.uvBuffer[t]->LoadData(&obj.uv[t][0],sizeof(vec2)*obj.uv[t].size(),STATIC))
				{
					ostringstream oss;
					oss << "buffer id:"<<obj.vertexBuffer->id << " Couldn't upload vertex data" << endl;
					Logger::Write(oss.str());
				}
			}
			else
				obj.uvBuffer[t].reset();
		}
		

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