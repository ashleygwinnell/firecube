#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include <GL/GLU.h>
#include "FireCube.h"
using namespace FireCube;

Material::Material() : tex(0)
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
Model::Model()
{

}
Model::~Model()
{

}

bool Model::Load3ds(const string &filename,const string &objName)
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

	ProcessChunk(buffer,objName);	
	for (DWORD i=0;i<mesh.size();i++)
	{
		Mesh *sm=&mesh[i];		
		for (DWORD j=0;j<sm->face.size();j++)
		{
			DWORD idx=sm->face[j].v[0];
			sm->face[j].v[0]=face[idx].v[0];
			sm->face[j].v[1]=face[idx].v[1];
			sm->face[j].v[2]=face[idx].v[2];
		}
		sm->indexBuffer.Create();
		sm->indexBuffer.LoadIndexData(&sm->face[0],sm->face.size()*3,STATIC);
	}
	
	delete [] buffer;

	f.close();

	//CalcNormals();
	return true;
}
DWORD Model::ProcessChunk(char *buffer,const string &objName)
{
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
		if (strcmp(buffer+i,objName.c_str())==0)
		{
			name=buffer+i;			
			i+=(DWORD)strlen(buffer+i)+1;
		}
		else
			i+=len;
		break;
	case OBJ_MESH:
		break;
	case MESH_VERTICES:			
		float x,y,z;
		numVertices=*(WORD*)(buffer+i);
		vertex.resize(numVertices);
		i+=2;
		for (j=0;j<numVertices;j++)
		{
			x=*(float*)(buffer+i);
			z=-*(float*)(buffer+i+4);
			y=*(float*)(buffer+i+8);
			vertex[j].Set(x,y,z);
			i+=12;
		}
		vertexBuffer.Create();
		vertexBuffer.LoadData(&vertex[0],sizeof(vec3)*numVertices,STATIC);
		break;
	case MESH_FACES:			
		numFaces=*(WORD*)(buffer+i);
		i+=2;
		face.resize(numFaces);
		for (j=0;j<numFaces;j++)
		{
			face[j].v[0]=*(WORD*)(buffer+i);
			face[j].v[1]=*(WORD*)(buffer+i+2);
			face[j].v[2]=*(WORD*)(buffer+i+4);
			i+=8;
		}
		break;
	case MESH_TEX_VERT:
		numtexcoords=*(WORD*)(buffer+i);
		uv.resize(numtexcoords);
		i+=2;
		for (j=0;j<numtexcoords;j++)
		{
			uv[j].x=*(float*)(buffer+i);
			uv[j].y=*(float*)(buffer+i+4);
			i+=8;
		}
		uvBuffer.Create();
		uvBuffer.LoadData(&uv[0],sizeof(vec2)*numtexcoords,STATIC);
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
		this->mesh.push_back(sm);
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
	case MAT_TEXMAP:
		i+=8;
		break;
	case MAT_TEXFLNM:
		matPtr=&material[0];
		SDL_Surface *image;
		image=IMG_Load(buffer+i);
		if(image) 
		{
			GLenum format;
			glGenTextures(1,&(matPtr->tex));
						
			if (image->format->BytesPerPixel==4)
				format=GL_RGBA;
			if (image->format->BytesPerPixel==3)
				format=GL_RGB;

			glBindTexture(GL_TEXTURE_2D,matPtr->tex);
			gluBuild2DMipmaps(GL_TEXTURE_2D,image->format->BytesPerPixel,image->w,image->h,format,GL_UNSIGNED_BYTE,image->pixels);			
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);			
			SDL_FreeSurface(image);
		}		
		i+=(DWORD)strlen(buffer+i)+1;
		break;
	default:	
		i=len;
		break;

	}
	while (i<len)
		i+=ProcessChunk(buffer+i,objName);

	return len;
}
Material *Model::GetMaterialByName(const string &name)
{
	for (DWORD i=0;i<material.size();i++)
	{
		if (material[i].name==name)
			return &material[i];
	}
	return NULL;
}