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

M3dsLoader::M3dsLoader()
{

}
bool M3dsLoader::Load(const string &filename)
{
	ifstream f(filename.c_str(),ios::in|ios::binary|ios::ate);
	if (!f.is_open())
		return false;
	
	DWORD l=f.tellg();	
	buffer.resize(l);
	f.seekg(0,ios_base::beg);
	f.read(&buffer[0],l);
	curPos=&buffer[0];
	ReadMainChunk();
	
	vector<pair<pair<DWORD,DWORD>,string>>::iterator meshMat;
	for (meshMat=meshMaterial.begin();meshMat!=meshMaterial.end();meshMat++)
		object[meshMat->first.first].mesh[meshMat->first.second].material=GetMaterialByName(meshMat->second);
	vector<pair<DWORD,mat4>>::iterator objMatrix;
	for (DWORD k=0;k<object.size();k++)
	{
		if (object[k].mesh.size()==0) // No material specified, create a default one.
		{
			Material mat(new MaterialResource);
			mat->ambient.Set(0,0,0,1);
			mat->diffuse.Set(1,1,1,1);
			mat->specular.Set(0,0,0,1);
			materials.push_back(mat);
			object[k].mesh.push_back(Mesh());
			Mesh &mesh=object[k].mesh.back();
			mesh.material=mat;
			mesh.face=object[k].face;
		}
	}
	return true;
}
Node M3dsLoader::GenerateSceneGraph()
{
	Node ret(new NodeResource);
	for (DWORD i=0;i<object.size();i++)
	{
		Geometry geom(new GeometryResource);
		geom->vertex=object[i].vertex;
		geom->diffuseUV=object[i].uv;
		geom->material=materials;
		for (DWORD j=0;j<object[i].mesh.size();j++)
		{
			Mesh &mesh=object[i].mesh[j];
			geom->surface.push_back(Surface());
			Surface &surface=geom->surface.back();
			surface.material=mesh.material;
			for (DWORD k=0;k<mesh.face.size();k++)
			{
				Face f;
				f.v[0]=mesh.face[k].v[0];
				f.v[1]=mesh.face[k].v[1];
				f.v[2]=mesh.face[k].v[2];
				surface.face.push_back(f);
				geom->face.push_back(f);
			}
		}
		geom->CalculateNormals();
		geom->UpdateBuffers();
		Node node(object[i].name);		
		node.AddGeometry(geom);
		ret.AddChild(node);
	}
	return ret;
}
void M3dsLoader::ReadMainChunk()
{	
	WORD id=*(WORD*)curPos;
	DWORD len=*(DWORD*)(curPos+2)-6;	
	curPos+=6;	
	char *startPos=curPos;
	if (id!=MAIN3DS)
		return;	
	while ((DWORD)(curPos-startPos)<len)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==EDIT3DS)		
			ReadEdit3dsChunk(subLen);		
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadEdit3dsChunk(DWORD length)
{
	char *startPos=curPos;
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;
		curPos+=6;
		if (subId==EDIT_OBJECT)		
			ReadObjectChunk(subLen);		
		else if (subId==EDIT_MATERIAL)		
			ReadMaterialListChunk(subLen);		
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadObjectChunk(DWORD length)
{
	char *startPos=curPos;	
	string name=curPos;
	curPos+=name.size()+1;
	
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==OBJ_TRIMESH)
		{	
			object.push_back(Object());
			object.back().name=name;		
			ReadTriMeshChunk(subLen);
		}
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadTriMeshChunk(DWORD length)
{
	char *startPos=curPos;
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;
		curPos+=6;
		if (subId==TRI_VERTEXLIST)
			ReadVerticesListChunk();
		else if (subId==TRI_FACELIST)
			ReadFacesListChunk(subLen);
		else if (subId==TRI_TEXCOORDLIST)
			ReadTexCoordListChunk();
		else if (subId==TRI_MATRIX)
			ReadObjectMatrixChunk();
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadVerticesListChunk()
{
	Object &obj=object.back();
	obj.vertex.resize(*(WORD*)curPos);
	curPos+=2;
	for (DWORD i=0;i<obj.vertex.size();i++)
	{
		obj.vertex[i].x=*(float*)curPos;
		curPos+=4;
		obj.vertex[i].z=-*(float*)curPos;
		curPos+=4;
		obj.vertex[i].y=*(float*)curPos;
		curPos+=4;
	}
}
void M3dsLoader::ReadFacesListChunk(DWORD length)
{
	Object &obj=object.back();
	char *startPos=curPos;	
	obj.face.resize(*(WORD*)curPos);
	curPos+=2;
	for (DWORD i=0;i<obj.face.size();i++)
	{
		for (DWORD j=0;j<3;j++)
		{
			obj.face[i].v[j]=*(WORD*)curPos;
			curPos+=2;
		}
		curPos+=2;
	}
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==TRI_MATERIAL)
			ReadMaterialFaceList();
		else			
			curPos+=subLen;
	}
}
void M3dsLoader::ReadTexCoordListChunk()
{	
	Object &obj=object.back();
	obj.uv.resize(*(WORD*)curPos);
	curPos+=2;
	for (DWORD i=0;i<obj.uv.size();i++)
	{
		obj.uv[i].x=*(float*)curPos;
		curPos+=4;
		obj.uv[i].y=1.0f-*(float*)curPos;
		curPos+=4;
	}	
}
void M3dsLoader::ReadMaterialFaceList()
{	
	Object &obj=object.back();
	string matName=curPos;
	curPos+=matName.size()+1;
	obj.mesh.push_back(Mesh());
	Mesh &mesh=obj.mesh.back();		
	meshMaterial.push_back(make_pair(make_pair(object.size()-1,obj.mesh.size()-1),matName));
	WORD count=*(WORD*)curPos;
	curPos+=2;
	for (DWORD i=0;i<count;i++)
	{
		mesh.face.push_back(obj.face[*(WORD*)curPos]);
		curPos+=2;
	}	
}
void M3dsLoader::ReadObjectMatrixChunk()
{
	float *arr=(float*)curPos;
	mat4 mat;
	mat.m[0]=arr[0];
	mat.m[4]=arr[1];
	mat.m[8]=arr[2];
	mat.m[12]=arr[3];
	mat.m[1]=arr[4];
	mat.m[5]=arr[5];
	mat.m[9]=arr[6];
	mat.m[13]=arr[7];
	mat.m[2]=arr[8];
	mat.m[6]=arr[9];
	mat.m[10]=arr[10];
	mat.m[14]=arr[11];
	objectMatrix.push_back(make_pair(object.size()-1,mat));
	curPos+=4*12;
}
void M3dsLoader::ReadMaterialListChunk(DWORD length)
{
	char *startPos=curPos;

	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==MAT_NAME)
			ReadMaterialNameChunk();
		else if (subId==MAT_AMBIENT)
			ReadMaterialColorChunk(subLen,curMaterial->ambient);
		else if (subId==MAT_DIFFUSE)
			ReadMaterialColorChunk(subLen,curMaterial->diffuse);
		else if (subId==MAT_SPECULAR)
			ReadMaterialColorChunk(subLen,curMaterial->specular);
		else if (subId==MAT_TEXMAP)
			ReadMaterialTexMapChunk(subLen,curMaterial->diffuseTexture);
		else if (subId==MAT_SHININESS)
			ReadMaterialShininessChunk(subLen,curMaterial->shininess);
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadMaterialNameChunk()
{	
	curMaterial=Material(new MaterialResource);
	materials.push_back(curMaterial);	
	curMaterial->name=curPos;
	curPos+=curMaterial->name.size()+1;
}
vec4 M3dsLoader::ReadColorFChunk()
{
	vec4 ret;
	ret.x=*(float*)curPos;
	curPos+=4;
	ret.y=*(float*)curPos;
	curPos+=4;
	ret.z=*(float*)curPos;
	curPos+=4;
	return ret;
}
vec4 M3dsLoader::ReadColorBChunk()
{
	vec4 ret;
	ret.x=(float)(*(unsigned char*)curPos)/255.0f;
	curPos++;
	ret.y=(float)(*(unsigned char*)curPos)/255.0f;
	curPos++;
	ret.z=(float)(*(unsigned char*)curPos)/255.0f;
	curPos++;
	return ret;
}
void M3dsLoader::ReadMaterialColorChunk(DWORD length,vec4 &color)
{
	char *startPos=curPos;
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==COL_RGB_B)
			color=ReadColorBChunk();
		else if (subId==COL_RGB_F)
			color=ReadColorFChunk();
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadMaterialShininessChunk(DWORD length,float &shininess)
{
	char *startPos=curPos;
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==PERCENTAGE_B)
			shininess=ReadPercentageBChunk();
		else if (subId==PERCENTAGE_F)
			shininess=ReadPercentageFChunk();
		else
			curPos+=subLen;
	}
}
string M3dsLoader::ReadMapNameChunk()
{
	string ret=curPos;
	curPos+=ret.size()+1;
	return ret;
}
void M3dsLoader::ReadMaterialTexMapChunk(DWORD length,Texture &texture)
{
	char *startPos=curPos;
	while ((DWORD)(curPos-startPos)<length)
	{
		WORD subId=*(WORD*)curPos;
		DWORD subLen=*(DWORD*)(curPos+2)-6;	
		curPos+=6;
		if (subId==MAT_MAPNAME)					
			texture=Renderer::GetTextureManager().Create(ReadMapNameChunk());		
		else
			curPos+=subLen;
	}
}
Material M3dsLoader::GetMaterialByName(const string &name)
{
	vector<Material>::iterator i=materials.begin();
	for (;i!=materials.end();i++)
	{
		if ((*i)->name==name)
			return *i;
	}
	return Material();
}
float M3dsLoader::ReadPercentageBChunk()
{
	float ret=*(WORD*)curPos;
	curPos+=2;
	return ret;
}
float M3dsLoader::ReadPercentageFChunk()
{
	float ret=*(float*)curPos;
	curPos+=4;
	return ret;
}