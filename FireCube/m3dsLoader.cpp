#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;
#include <dae.h>
#include <dom/domCOLLADA.h>
#include "ModelLoaders.h"


M3dsLoader::M3dsLoader(ModelResource *model) : model(model)
{

}
bool M3dsLoader::Load(const string &filename)
{
	ifstream f(filename.c_str(),ios::in|ios::binary|ios::ate);
	if (!f.is_open())
		return false;
	
	DWORD l=f.tellg();
	char *buffer=new char[l];
	f.seekg(0,ios_base::beg);
	f.read(buffer,l);

	ProcessChunk(buffer);	
	for (DWORD k=0;k<model->object.size();k++)
	{
		for (DWORD i=0;i<model->object[k].mesh.size();i++)
		{
			Mesh *sm=&model->object[k].mesh[i];		
			for (DWORD j=0;j<sm->face.size();j++)
			{
				DWORD idx=sm->face[j].v[0];
				sm->face[j]=model->object[k].face[idx];				
			}			
		}
	}
	model->CalculateNormals();
	model->UpdateBuffers();
	delete [] buffer;		
	return true;
}
DWORD M3dsLoader::ProcessChunk(char *buffer)
{
	Object *curObject=NULL;
	if (model->object.size()>0)
		curObject=&model->object[model->object.size()-1];
	DWORD i=0,j=0;	
	Material mat;
	Material matPtr;
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
		model->object.push_back(Object());
		model->object[model->object.size()-1].name=buffer+i;			
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
		curObject->uv[0].resize(numtexcoords);
		i+=2;
		for (j=0;j<numtexcoords;j++)
		{
			curObject->uv[0][j].x=*(float*)(buffer+i);
			curObject->uv[0][j].y=1.0f-*(float*)(buffer+i+4);
			i+=8;
		}		
		break;
	case MESH_MATERIAL:		
		sm.material=model->GetMaterialByName(buffer+i);
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
		mat=Material(new MaterialResource);
		mat->name=buffer+i;		
		model->material.insert(model->material.begin(),mat);		
		i+=(DWORD)strlen(buffer+i)+1;		
		break;
	case MAT_AMBIENT:
		i+=6;
		matPtr=model->material[0];		
		matPtr->ambient=vec4((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f,1.0f);
		i+=3;
		break;
	case MAT_DIFFUSE:
		i+=6;
		matPtr=model->material[0];		
		matPtr->diffuse=vec4((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f,1.0f);
		i+=3;
		break;
	case MAT_SPECULAR:
		i+=6;
		matPtr=model->material[0];		
		matPtr->specular=vec4((float)(*(BYTE*)(buffer+i))/255.0f,(float)(*(BYTE*)(buffer+i+1))/255.0f,(float)(*(BYTE*)(buffer+i+2))/255.0f,1.0f);
		i+=3;
		break;
	case MAT_SHININESS:
		i+=6;
		i+=2;
		break;
	case MAT_SHIN2PCT:
		i+=6;
		matPtr=model->material[0];
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
		matPtr=model->material[0];						
		matPtr->texture[0]=Renderer::GetTextureManager().Create(buffer+i);		
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