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
#include "RenderQueue.h"
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
	
	unsigned int l=(unsigned int)f.tellg();	
	buffer.resize(l);
	f.seekg(0,ios_base::beg);
	f.read(&buffer[0],l);
	curPos=&buffer[0];
	ReadMainChunk();
	
	vector<pair<pair<unsigned int,unsigned int>,string>>::iterator meshMat;
	for (meshMat=meshMaterial.begin();meshMat!=meshMaterial.end();meshMat++)
		object[meshMat->first.first].mesh[meshMat->first.second].material=GetMaterialByName(meshMat->second);
	vector<pair<unsigned int,mat4>>::iterator objMatrix;
	for (unsigned int k=0;k<object.size();k++)
	{
		if (object[k].mesh.size()==0) // No material specified, create a default one.
		{
			Material mat;
			mat.Create();
			mat.SetAmbientColor(vec4(0,0,0,1));
			mat.SetDiffuseColor(vec4(1,1,1,1));
			mat.SetSpecularColor(vec4(0,0,0,1));
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
	for (unsigned int i=0;i<object.size();i++)
	{
		Geometry geom;
		geom.Create();
		geom.GetVertices()=object[i].vertex;
		geom.GetDiffuseUV()=object[i].uv;
		geom.GetMaterials()=materials;
		for (unsigned int j=0;j<object[i].mesh.size();j++)
		{
			Mesh &mesh=object[i].mesh[j];
			geom.GetSurfaces().push_back(Surface());
			Surface &surface=geom.GetSurfaces().back();
			surface.material=mesh.material;
			for (unsigned int k=0;k<mesh.face.size();k++)
			{
				Face f;
				f.v[0]=mesh.face[k].v[0];
				f.v[1]=mesh.face[k].v[1];
				f.v[2]=mesh.face[k].v[2];
				surface.face.push_back(f);
				geom.GetFaces().push_back(f);
			}
		}
		geom.CalculateNormals();
		geom.CalculateTangents();
		geom.UpdateBuffers();
		Node node(object[i].name);		
		node.AddGeometry(geom);
		ret.AddChild(node);
	}
	ret.SetTechnique("default");
	return ret;
}
void M3dsLoader::ReadMainChunk()
{	
	unsigned short int id=*(unsigned short int*)curPos;
	unsigned int len=*(unsigned int*)(curPos+2)-6;	
	curPos+=6;	
	char *startPos=curPos;
	if (id!=MAIN3DS)
		return;	
	while ((unsigned int)(curPos-startPos)<len)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
		curPos+=6;
		if (subId==EDIT3DS)		
			ReadEdit3dsChunk(subLen);		
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadEdit3dsChunk(unsigned int length)
{
	char *startPos=curPos;
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;
		curPos+=6;
		if (subId==EDIT_OBJECT)		
			ReadObjectChunk(subLen);		
		else if (subId==EDIT_MATERIAL)		
			ReadMaterialListChunk(subLen);		
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadObjectChunk(unsigned int length)
{
	char *startPos=curPos;	
	string name=curPos;
	curPos+=name.size()+1;
	
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
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
void M3dsLoader::ReadTriMeshChunk(unsigned int length)
{
	char *startPos=curPos;
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;
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
	obj.vertex.resize(*(unsigned short int*)curPos);
	curPos+=2;
	for (unsigned int i=0;i<obj.vertex.size();i++)
	{
		obj.vertex[i].x=*(float*)curPos;
		curPos+=4;
		obj.vertex[i].z=-*(float*)curPos;
		curPos+=4;
		obj.vertex[i].y=*(float*)curPos;
		curPos+=4;
	}
}
void M3dsLoader::ReadFacesListChunk(unsigned int length)
{
	Object &obj=object.back();
	char *startPos=curPos;	
	obj.face.resize(*(unsigned short int*)curPos);
	curPos+=2;
	for (unsigned int i=0;i<obj.face.size();i++)
	{
		for (unsigned int j=0;j<3;j++)
		{
			obj.face[i].v[j]=*(unsigned short int*)curPos;
			curPos+=2;
		}
		curPos+=2;
	}
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
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
	obj.uv.resize(*(unsigned short int*)curPos);
	curPos+=2;
	for (unsigned int i=0;i<obj.uv.size();i++)
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
	unsigned short int count=*(unsigned short int*)curPos;
	curPos+=2;
	for (unsigned int i=0;i<count;i++)
	{
		mesh.face.push_back(obj.face[*(unsigned short int*)curPos]);
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
void M3dsLoader::ReadMaterialListChunk(unsigned int length)
{
	char *startPos=curPos;

	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
		curPos+=6;
		if (subId==MAT_NAME)
			ReadMaterialNameChunk();
		else if (subId==MAT_AMBIENT)
			curMaterial.SetAmbientColor(ReadMaterialColorChunk(subLen));
		else if (subId==MAT_DIFFUSE)
			curMaterial.SetDiffuseColor(ReadMaterialColorChunk(subLen));
		else if (subId==MAT_SPECULAR)
			curMaterial.SetSpecularColor(ReadMaterialColorChunk(subLen));
		else if (subId==MAT_TEXMAP)
			curMaterial.SetDiffuseTexture(ReadMaterialTexMapChunk(subLen));
		else if (subId==MAT_SHININESS)
			curMaterial.SetShininess(ReadMaterialShininessChunk(subLen));
		else
			curPos+=subLen;
	}
}
void M3dsLoader::ReadMaterialNameChunk()
{	
	curMaterial.Create();
	materials.push_back(curMaterial);	
	curMaterial.SetName(curPos);
	curPos+=curMaterial.GetName().size()+1;
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
vec4 M3dsLoader::ReadMaterialColorChunk(unsigned int length)
{
	vec4 ret;
	char *startPos=curPos;
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
		curPos+=6;
		if (subId==COL_RGB_B)
			ret=ReadColorBChunk();
		else if (subId==COL_RGB_F)
			ret=ReadColorFChunk();
		else
			curPos+=subLen;
	}
	return ret;
}
float M3dsLoader::ReadMaterialShininessChunk(unsigned int length)
{
	float ret;
	char *startPos=curPos;
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
		curPos+=6;
		if (subId==PERCENTAGE_B)
			ret=ReadPercentageBChunk();
		else if (subId==PERCENTAGE_F)
			ret=ReadPercentageFChunk();
		else
			curPos+=subLen;
	}
	return ret;
}
string M3dsLoader::ReadMapNameChunk()
{
	string ret=curPos;
	curPos+=ret.size()+1;
	return ret;
}
Texture M3dsLoader::ReadMaterialTexMapChunk(unsigned int length)
{
	Texture ret;	
	char *startPos=curPos;
	while ((unsigned int)(curPos-startPos)<length)
	{
		unsigned short int subId=*(unsigned short int*)curPos;
		unsigned int subLen=*(unsigned int*)(curPos+2)-6;	
		curPos+=6;
		if (subId==MAT_MAPNAME)					
			ret=Renderer::GetTextureManager().Create(ReadMapNameChunk());		
		else
			curPos+=subLen;
	}
	return ret;
}
Material M3dsLoader::GetMaterialByName(const string &name)
{
	vector<Material>::iterator i=materials.begin();
	for (;i!=materials.end();i++)
	{
		if ((*i).GetName()==name)
			return *i;
	}
	return Material();
}
float M3dsLoader::ReadPercentageBChunk()
{
	float ret=*(unsigned short int*)curPos;
	curPos+=2;
	return ret;
}
float M3dsLoader::ReadPercentageFChunk()
{
	float ret=*(float*)curPos;
	curPos+=4;
	return ret;
}