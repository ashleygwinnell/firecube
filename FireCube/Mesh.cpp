#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"
#include <GL/GLU.h>
#include "FireCube.h"
using namespace FireCube;

Material::Material() : tex()
{
	
}
Material::~Material()
{

}
Face::Face()
{
	
}
Face::~Face()
{

}
Mesh::Mesh() : material(NULL)
{
	
}
Mesh::~Mesh()
{

}
ModelResource::ModelResource()
{

}
ModelResource::~ModelResource()
{	
}

bool ModelResource::Load(const string &filename)
{
	string::size_type d;
	d=filename.find_last_of(".");
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="3ds")
		{		
			ifstream f(filename.c_str(),ios::in|ios::binary|ios::ate);
			if (!f.is_open())
			{		
				return false;
			}	
			DWORD l=f.tellg();
			char *buffer=new char[l];
			f.seekg(0,ios_base::beg);
			f.read(buffer,l);

			ProcessChunk(buffer);	
			for (DWORD k=0;k<object.size();k++)
			{
				for (DWORD i=0;i<object[k].mesh.size();i++)
				{
					Mesh *sm=&object[k].mesh[i];		
					for (DWORD j=0;j<sm->face.size();j++)
					{
						DWORD idx=sm->face[j].v[0];
						sm->face[j]=object[k].face[idx];				
					}
					sm->indexBuffer.Create();
					vector<DWORD> tmp;
					tmp.resize(sm->face.size()*3);
					for (DWORD f=0;f<sm->face.size();f++)
					{
						tmp[f*3+0]=sm->face[f].v[0];
						tmp[f*3+1]=sm->face[f].v[1];
						tmp[f*3+2]=sm->face[f].v[2];
					}
					sm->indexBuffer.LoadIndexData(&tmp[0],tmp.size(),STATIC);
				}
			}
			CalculateNormals();

			delete [] buffer;		
			return true;
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
			n.Normalize();
			object[k].face[f].normal=n;
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
		object[k].normalBuffer.Create();
		object[k].normalBuffer.LoadData(&object[k].normal[0],sizeof(vec3)*object[k].normal.size(),STATIC);
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
		object[k].vertexBuffer.LoadData(&object[k].vertex[0],sizeof(vec3)*object[k].vertex.size(),STATIC);
	}
	CalculateNormals();
}
DWORD ModelResource::ProcessChunk(char *buffer)
{
	Object *curObject=NULL;
	if (object.size()>0)
		curObject=&object[object.size()-1];
	DWORD i=0,j=0;
	Material mat;
	Material *matPtr;
	Mesh sm;
	DWORD numtexcoords,numFaces,numVertices;
	WORD id=*(WORD*)(buffer+i);
	DWORD len=*(DWORD*)(buffer+i+2);		
	i+=6;
	switch (id)
	{
	case PRIMARY:
		break;
	case EDIT3DS:
		break;
	case NAMED_OBJECT:
			object.push_back(Object());
			object[object.size()-1].name=buffer+i;			
			i+=(DWORD)strlen(buffer+i)+1;
		break;
	case OBJ_MESH:
		break;
	case MESH_VERTICES:			
		float x,y,z;
		numVertices=*(WORD*)(buffer+i);
		curObject->vertex.resize(numVertices);
		i+=2;
		for (j=0;j<numVertices;j++)
		{
			x=*(float*)(buffer+i);
			z=-*(float*)(buffer+i+4);
			y=*(float*)(buffer+i+8);
			curObject->vertex[j].Set(x,y,z);
			i+=12;
		}
		curObject->vertexBuffer.Create();
		curObject->vertexBuffer.LoadData(&curObject->vertex[0],sizeof(vec3)*numVertices,STATIC);
		break;
	case MESH_FACES:			
		numFaces=*(WORD*)(buffer+i);
		i+=2;
		curObject->face.resize(numFaces);
		for (j=0;j<numFaces;j++)
		{
			curObject->face[j].v[0]=*(WORD*)(buffer+i);
			curObject->face[j].v[1]=*(WORD*)(buffer+i+2);
			curObject->face[j].v[2]=*(WORD*)(buffer+i+4);
			i+=8;
		}
		break;
	case MESH_TEX_VERT:
		numtexcoords=*(WORD*)(buffer+i);
		curObject->uv.resize(numtexcoords);
		i+=2;
		for (j=0;j<numtexcoords;j++)
		{
			curObject->uv[j].x=*(float*)(buffer+i);
			curObject->uv[j].y=1.0f-*(float*)(buffer+i+4);
			i+=8;
		}
		curObject->uvBuffer.Create();
		curObject->uvBuffer.LoadData(&curObject->uv[0],sizeof(vec2)*numtexcoords,STATIC);
		break;
	case MESH_MATERIAL:		
		sm.material=GetMaterialByName(buffer+i);
		i+=(DWORD)strlen(buffer+i)+1;
		numFaces=*(WORD*)(buffer+i);
		i+=2;
		sm.face.resize(numFaces);
		for (j=0;j<numFaces;j++)
		{
			sm.face[j].v[0]=*(WORD*)(buffer+i);
			i+=2;
		}			
		curObject->mesh.push_back(sm);
		break;
	case MATERIAL:
		break;
	case MAT_NAME:		
		mat.name=buffer+i;		
		this->material.insert(this->material.begin(),mat);		
		i+=(DWORD)strlen(buffer+i)+1;
		break;
	case MAT_AMBIENT:
		i+=6;
		matPtr=&this->material[0];		
		matPtr->ambient=vec3((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f);
		i+=3;
		break;
	case MAT_DIFFUSE:
		i+=6;
		matPtr=&this->material[0];		
		matPtr->diffuse=vec3((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f);
		i+=3;
		break;
	case MAT_SPECULAR:
		i+=6;
		matPtr=&this->material[0];		
		matPtr->specular=vec3((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f);
		i+=3;
		break;
	case MAT_SHININESS:
		i+=6;
		i+=2;
		break;
	case MAT_SHIN2PCT:
		i+=6;
		matPtr=&this->material[0];
		matPtr->shininess=(float)(*(WORD*)(buffer+i));
		i+=2;
		break;
	case MAT_SHIN3PCT:
		i+=6;
		i+=2;
		break;
	case MAT_TEXMAP:
		i+=8;
		break;
	case MAT_TEXFLNM:
		matPtr=&material[0];						
		matPtr->tex=Application::GetContext().textureManager->Create(buffer+i);		
		i+=(DWORD)strlen(buffer+i)+1;
		break;
	default:	
		i=len;
		break;

	}
	while (i<len)
		i+=ProcessChunk(buffer+i);

	return len;
}
Material *ModelResource::GetMaterialByName(const string &name)
{
	for (DWORD i=0;i<material.size();i++)
	{
		if (material[i].name==name)
			return &material[i];
	}
	return NULL;
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
					if (curObject->vertex[k]==v)
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
