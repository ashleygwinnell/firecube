#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/ResourcePool.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Texture.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Renderer.h"
#include "Dependencies/tinyxml.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Geometry/ModelLoaders.h"

using namespace FireCube;

M3dsLoader::M3dsLoader()
{

}
bool M3dsLoader::Load(const string &filename, ModelLoadingOptions options)
{
	ifstream f(filename.c_str(), ios::in | ios::binary | ios::ate);
	if (!f.is_open())
		return false;

	unsigned int l = (unsigned int)f.tellg();
	buffer.resize(l);
	f.seekg(0, ios_base::beg);
	f.read(&buffer[0], l);
	curPos = &buffer[0];
	ReadMainChunk();

	vector<pair<pair<unsigned int, unsigned int>, string>>::iterator meshMat;
	for (meshMat = meshMaterial.begin(); meshMat != meshMaterial.end(); meshMat++)
		object[meshMat->first.first].mesh[meshMat->first.second].material = GetMaterialByName(meshMat->second);
	vector<pair<unsigned int, mat4>>::iterator objMatrix;
	for (unsigned int k = 0; k < object.size(); k++)
	{
		if (object[k].mesh.size() == 0) // No material specified, create a default one.
		{
			MaterialPtr mat(new Material);
			mat->SetAmbientColor(vec3(0, 0, 0));
			mat->SetDiffuseColor(vec3(1, 1, 1));
			mat->SetSpecularColor(vec3(0, 0, 0));
			materials.push_back(mat);
			object[k].mesh.push_back(Mesh());
			Mesh &mesh = object[k].mesh.back();
			mesh.material = mat;
			mesh.face = object[k].face;
		}
	}
	this->options = options;
	return true;
}
NodePtr M3dsLoader::GenerateSceneGraph()
{
	NodePtr ret(new Node);
	for (unsigned int i = 0; i < object.size(); i++)
	{   
		NodePtr objectNode(new Node(object[i].name));
		ret->AddChild(objectNode);
		for (unsigned int j = 0; j < object[i].mesh.size(); j++)
		{
			GeometryPtr geom(new Geometry);

			geom->GetVertices() = object[i].vertex;
			geom->GetDiffuseUV() = object[i].uv;			
			if (options.flipU || options.flipV)
			{
				for (unsigned int k = 0; k < geom->GetDiffuseUV().size(); k++)
				{
					if (options.flipU)
						geom->GetDiffuseUV()[k].x = 1 - geom->GetDiffuseUV()[k].x;
					if (options.flipV)
						geom->GetDiffuseUV()[k].y = 1 - geom->GetDiffuseUV()[k].y;
				}
			}
			Mesh &mesh = object[i].mesh[j];
			geom->SetMaterial(mesh.material);
			for (unsigned int k = 0; k < mesh.face.size(); k++)
			{
				Face f;
				f.v[0] = mesh.face[k].v[0];
				f.v[1] = mesh.face[k].v[1];
				f.v[2] = mesh.face[k].v[2];
				geom->GetFaces().push_back(f);
			}
			geom->CopyFacesToIndexBuffer();
			geom->SetPrimitiveType(TRIANGLES);
			geom->SetPrimitiveCount(geom->GetFaces().size());
			geom->SetVertexCount(geom->GetFaces().size() * 3);
			geom->CalculateNormals();
			geom->CalculateTangents();
			geom->UpdateBuffers();
			geom->CalculateBoundingBox();
			ostringstream oss;
			oss << object[i].name << "-surface-" << j;
			NodePtr node(new Node(oss.str()));
			node->SetGeometry(geom);
			objectNode->AddChild(node);
		}        
	}    
	return ret;
}
void M3dsLoader::ReadMainChunk()
{
	unsigned short int id = *(unsigned short int*)curPos;
	unsigned int len = *(unsigned int*)(curPos + 2) - 6;
	curPos += 6;
	char *startPos = curPos;
	if (id != MAIN3DS)
		return;
	while ((unsigned int)(curPos - startPos) < len)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == EDIT3DS)
			ReadEdit3dsChunk(subLen);
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadEdit3dsChunk(unsigned int length)
{
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == EDIT_OBJECT)
			ReadObjectChunk(subLen);
		else if (subId == EDIT_MATERIAL)
			ReadMaterialListChunk(subLen);
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadObjectChunk(unsigned int length)
{
	char *startPos = curPos;
	string name = curPos;
	curPos += name.size() + 1;

	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == OBJ_TRIMESH)
		{
			object.push_back(Object());
			object.back().name = name;
			ReadTriMeshChunk(subLen);
		}
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadTriMeshChunk(unsigned int length)
{
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == TRI_VERTEXLIST)
			ReadVerticesListChunk();
		else if (subId == TRI_FACELIST)
			ReadFacesListChunk(subLen);
		else if (subId == TRI_TEXCOORDLIST)
			ReadTexCoordListChunk();
		else if (subId == TRI_MATRIX)
			ReadObjectMatrixChunk();
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadVerticesListChunk()
{
	Object &obj = object.back();
	obj.vertex.resize(*(unsigned short int*)curPos);
	curPos += 2;
	for (unsigned int i = 0; i < obj.vertex.size(); i++)
	{
		obj.vertex[i].x = *(float*)curPos;
		curPos += 4;
		obj.vertex[i].z = -*(float*)curPos;
		curPos += 4;
		obj.vertex[i].y = *(float*)curPos;
		curPos += 4;
	}
}
void M3dsLoader::ReadFacesListChunk(unsigned int length)
{
	Object &obj = object.back();
	char *startPos = curPos;
	obj.face.resize(*(unsigned short int*)curPos);
	curPos += 2;
	for (unsigned int i = 0; i < obj.face.size(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			obj.face[i].v[j] = *(unsigned short int*)curPos;
			curPos += 2;
		}
		curPos += 2;
	}
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == TRI_MATERIAL)
			ReadMaterialFaceList();
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadTexCoordListChunk()
{
	Object &obj = object.back();
	obj.uv.resize(*(unsigned short int*)curPos);
	curPos += 2;
	for (unsigned int i = 0; i < obj.uv.size(); i++)
	{
		obj.uv[i].x = *(float*)curPos;
		curPos += 4;
		obj.uv[i].y = 1.0f - *(float*)curPos;
		curPos += 4;
	}
}
void M3dsLoader::ReadMaterialFaceList()
{
	Object &obj = object.back();
	string matName = curPos;
	curPos += matName.size() + 1;
	obj.mesh.push_back(Mesh());
	Mesh &mesh = obj.mesh.back();
	meshMaterial.push_back(make_pair(make_pair(object.size() - 1, obj.mesh.size() - 1), matName));
	unsigned short int count = *(unsigned short int*)curPos;
	curPos += 2;
	for (unsigned int i = 0; i < count; i++)
	{
		mesh.face.push_back(obj.face[*(unsigned short int*)curPos]);
		curPos += 2;
	}
}
void M3dsLoader::ReadObjectMatrixChunk()
{
	float *arr = (float*)curPos;
	mat4 mat = mat4::identity;
	mat.m[0] = arr[0];
	mat.m[4] = arr[1];
	mat.m[8] = arr[2];
	mat.m[12] = arr[3];
	mat.m[1] = arr[4];
	mat.m[5] = arr[5];
	mat.m[9] = arr[6];
	mat.m[13] = arr[7];
	mat.m[2] = arr[8];
	mat.m[6] = arr[9];
	mat.m[10] = arr[10];
	mat.m[14] = arr[11];
	objectMatrix.push_back(make_pair(object.size() - 1, mat));
	curPos += 4 * 12;
}
void M3dsLoader::ReadMaterialListChunk(unsigned int length)
{
	char *startPos = curPos;

	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == MAT_NAME)
			ReadMaterialNameChunk();
		else if (subId == MAT_AMBIENT)
			curMaterial->SetAmbientColor(ReadMaterialColorChunk(subLen));
		else if (subId == MAT_DIFFUSE)
			curMaterial->SetDiffuseColor(ReadMaterialColorChunk(subLen));
		else if (subId == MAT_SPECULAR)
			curMaterial->SetSpecularColor(ReadMaterialColorChunk(subLen));
		else if (subId == MAT_TEXMAP)
			curMaterial->SetDiffuseTexture(ReadMaterialTexMapChunk(subLen));
		else if (subId == MAT_SHININESS)
			curMaterial->SetShininess(ReadMaterialShininessChunk(subLen));
		else
			curPos += subLen;
	}
}
void M3dsLoader::ReadMaterialNameChunk()
{
	curMaterial = MaterialPtr(new Material);
	materials.push_back(curMaterial);
	curMaterial->SetName(curPos);
	curPos += curMaterial->GetName().size() + 1;
}
vec3 M3dsLoader::ReadColorFChunk()
{
	vec3 ret;
	ret.x = *(float*)curPos;
	curPos += 4;
	ret.y = *(float*)curPos;
	curPos += 4;
	ret.z = *(float*)curPos;
	curPos += 4;
	return ret;
}
vec3 M3dsLoader::ReadColorBChunk()
{
	vec3 ret;
	ret.x = (float)(*(unsigned char*)curPos) / 255.0f;
	curPos++;
	ret.y = (float)(*(unsigned char*)curPos) / 255.0f;
	curPos++;
	ret.z = (float)(*(unsigned char*)curPos) / 255.0f;
	curPos++;
	return ret;
}
vec3 M3dsLoader::ReadMaterialColorChunk(unsigned int length)
{
	vec3 ret;
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == COL_RGB_B)
			ret = ReadColorBChunk();
		else if (subId == COL_RGB_F)
			ret = ReadColorFChunk();
		else
			curPos += subLen;
	}
	return ret;
}
float M3dsLoader::ReadMaterialShininessChunk(unsigned int length)
{
	float ret;
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == PERCENTAGE_B)
			ret = ReadPercentageBChunk();
		else if (subId == PERCENTAGE_F)
			ret = ReadPercentageFChunk();
		else
			curPos += subLen;
	}
	return ret;
}
string M3dsLoader::ReadMapNameChunk()
{
	string ret = curPos;
	curPos += ret.size() + 1;
	return ret;
}
TexturePtr M3dsLoader::ReadMaterialTexMapChunk(unsigned int length)
{
	TexturePtr ret;
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == MAT_MAPNAME)
			ret = Renderer::GetTexturePool().Create(ReadMapNameChunk());
		else
			curPos += subLen;
	}
	return ret;
}
MaterialPtr M3dsLoader::GetMaterialByName(const string &name)
{
	vector<MaterialPtr>::iterator i = materials.begin();
	for (; i != materials.end(); i++)
	{
		if ((*i)->GetName() == name)
			return *i;
	}
	return MaterialPtr();
}
float M3dsLoader::ReadPercentageBChunk()
{
	float ret = *(unsigned short int*)curPos;
	curPos += 2;
	return ret;
}
float M3dsLoader::ReadPercentageFChunk()
{
	float ret = *(float*)curPos;
	curPos += 4;
	return ret;
}