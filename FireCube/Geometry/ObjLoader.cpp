#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "Dependencies/GLee.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/ResourceManager.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Texture.h"
#include "Geometry/Geometry.h"
#include "Rendering/Renderer.h"
#include "Dependencies/tinyxml.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Geometry/ModelLoaders.h"

using namespace FireCube;

ObjLoader::ObjLoader()
{
	objects["default"] = Object();
	currentObject = &objects["default"];
	currentFaces = &objects["default"].facesWithoutMaterial;
}

void ObjLoader::Load(const string &filename)
{
	ifstream ifs(filename);
	string line;
	baseDir = ExtractDirectory(filename);
	while(getline(ifs, line))
	{		
		if (line.empty())
			continue;
		else if (line[0] == '#')
			continue;
		else if (line.size() >= 2 && line[0] == 'v' && line[1] == ' ')
			ParseVertexLine(line);
		else if (line.size() >= 2 && line[0] == 'f' && line[1] == ' ')
			ParseFaceLine(line);
		else if (line.size() >= 2 && line[0] == 'o' && line[1] == ' ')
			ParseObjectLine(line);
		else if (line.size() >= 3 && line[0] == 'v' && line[1] == 't' && line[2] == ' ')
			ParseTexCoordLine(line);
		else if (line.size() >= 3 && line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
			ParseNormalLine(line);
		else if (line.size() >= 6 && line.substr(0,6) == "usemtl")
			ParseUseMtlLine(line);
		else if (line.size() >= 6 && line.substr(0,6) == "mtllib")
			ParseMtlLibLine(line);
	}	
	if (objects["default"].facesWithoutMaterial.size() == 0 && objects["default"].materialFaces.size() == 0)
		objects.erase("default");
}
void ObjLoader::ParseVertexLine(const string &line)
{
	istringstream iss(line.substr(2));
	vec3 vertex;
	iss >> vertex.x;
	iss >> vertex.y;
	iss >> vertex.z;
	vertices.push_back(vertex);
}
void ObjLoader::ParseTexCoordLine(const string &line)
{
	istringstream iss(line.substr(3));
	vec2 uv;
	iss >> uv.x;
	iss >> uv.y;
	uv.y = 1.0f - uv.y;
	texCoords.push_back(uv);
}
void ObjLoader::ParseNormalLine(const string &line)
{
	istringstream iss(line.substr(3));
	vec3 normal;
	iss >> normal.x;
	iss >> normal.y;
	iss >> normal.z;
	normal.Normalize();
	normals.push_back(normal);
}
void ObjLoader::ParseFaceLine(const string &line)
{	
	Face f;
	for (int i = 0; i < 3; i++)
		f.v[i] = f.t[i] = f.n[i] = 0;
	int i1 = line.find_first_not_of(' ', 1);
	int i2 = line.find_first_of(' ', i1);
	ParseFaceEntry(line.substr(i1, i2 - i1), f.v[0], f.t[0], f.n[0], f.hasTextureCoordinates, f.hasNormal);
	i1 = line.find_first_not_of(' ', i2);
	i2 = line.find_first_of(' ', i1);	
	ParseFaceEntry(line.substr(i1, i2 - i1), f.v[1], f.t[1], f.n[1], f.hasTextureCoordinates, f.hasNormal);
	i1 = line.find_first_not_of(' ', i2);
	ParseFaceEntry(line.substr(i1), f.v[2], f.t[2], f.n[2], f.hasTextureCoordinates, f.hasNormal);
	currentFaces->push_back(f);
}
void ObjLoader::ParseFaceEntry(const string &entry, unsigned int &v, unsigned int &t, unsigned int &n, bool &hasTextureCoordinates, bool &hasNormal)
{
	int i1 = entry.find_first_of('/');
	if (i1 == string::npos)
	{
		istringstream iss(entry);
		iss >> v;
		hasTextureCoordinates = false;
		hasNormal = false;
		return;
	}

	int i2 = entry.find_first_of('/', i1 + 1);
	if (i2 == string::npos)
	{
		istringstream iss1(entry.substr(0, i1));
		istringstream iss2(entry.substr(i1 + 1));
		iss1 >> v;
		iss2 >> t;
		hasTextureCoordinates = true;
		hasNormal = false;
		return;
	}
	istringstream iss1(entry.substr(0, i1));	
	istringstream iss2(entry.substr(i2 + 1));
	iss1 >> v;	
	iss2 >> n;
	hasNormal = true;
	if (i2 - i1 - 1 > 0)
	{
		istringstream iss(entry.substr(i1 + 1, i2 - i1 - 1));
		iss >> t;
		hasTextureCoordinates = true;
	}
	else
		hasTextureCoordinates = false;
}
void ObjLoader::ParseObjectLine(const string &line)
{
	string name = line.substr(2);
	objects[name] = Object();
	currentObject = &objects[name];
	if (lastMaterial.empty())
		currentFaces = &objects[name].facesWithoutMaterial;
	else
	{
		objects[name].materialFaces[lastMaterial] = vector<Face>();
		currentFaces = &objects[name].materialFaces[lastMaterial];
	}

}
void ObjLoader::ParseUseMtlLine(const string &line)
{	
	lastMaterial = line.substr(7);
	if (currentObject->materialFaces.find(lastMaterial) == currentObject->materialFaces.end())
		currentObject->materialFaces[lastMaterial] = vector<Face>();
	currentFaces = &currentObject->materialFaces[lastMaterial];
}
void ObjLoader::ParseMtlLibLine(const string &line)
{
	ParseMaterialFile(line.substr(7));	
}
void ObjLoader::ParseMaterialFile(const string &filename)
{	
	ifstream ifs(baseDir + "\\" + filename);
	string line;

	while(getline(ifs, line))
	{		
		if (line.empty())
			continue;
		else if (line.size() >= 6 && line.substr(0,6) == "newmtl")
			ParseNewMtlLine(line);
		else if (line.size() >= 2 && line[0] == 'K' && line[1] == 'a')
			ParseAmbientColorLine(line);
		else if (line.size() >= 2 && line[0] == 'K' && line[1] == 'd')
			ParseDiffuseColorLine(line);
		else if (line.size() >= 2 && line[0] == 'K' && line[1] == 's')
			ParseSpecularColorLine(line);
		else if (line.size() >= 2 && line[0] == 'N' && line[1] == 's')
			ParseShininessLine(line);
		else if (line.size() >= 6 && line.substr(0,6) == "map_Kd")
			ParseDiffuseTextureMap(line);
		else if ((line.size() >= 4 && line.substr(0,4) == "bump") || (line.size() >= 8 && line.substr(0,8) == "map_bump"))
			ParseNormalTextureMap(line);		
	}
}
void ObjLoader::ParseNewMtlLine(const string &line)
{
	string name = line.substr(7);
	materials[name] = Material();
	currentMaterial = &materials[name];
}
void ObjLoader::ParseAmbientColorLine(const string &line)
{
	istringstream iss(line.substr(3));	
	iss >> currentMaterial->ambientColor.x;
	iss >> currentMaterial->ambientColor.y;
	iss >> currentMaterial->ambientColor.z;	 
}
void ObjLoader::ParseDiffuseColorLine(const string &line)
{
	istringstream iss(line.substr(3));	
	iss >> currentMaterial->diffuseColor.x;
	iss >> currentMaterial->diffuseColor.y;
	iss >> currentMaterial->diffuseColor.z;	 
}
void ObjLoader::ParseSpecularColorLine(const string &line)
{
	istringstream iss(line.substr(3));	
	iss >> currentMaterial->specularColor.x;
	iss >> currentMaterial->specularColor.y;
	iss >> currentMaterial->specularColor.z;	 
}
void ObjLoader::ParseDiffuseTextureMap(const string &line)
{
	currentMaterial->diffuseTextureName = line.substr(7);
}
void ObjLoader::ParseNormalTextureMap(const string &line)
{
	if (line.substr(0,4) == "bump")
		currentMaterial->normalTextureName = line.substr(5);
	else if (line.substr(0,8) == "map_bump")
		currentMaterial->normalTextureName = line.substr(9);
}
void ObjLoader::ParseShininessLine(const string &line)
{
	istringstream iss(line.substr(3));
	iss >> currentMaterial->shininess;
}
string ObjLoader::ExtractDirectory(const string &filename)
{
	int i = filename.find_last_of('\\');
	if (i == string::npos)
		return "";
	return filename.substr(0, i);
}
NodePtr ObjLoader::GenerateSceneGraph()
{
	NodePtr root(new Node);
	map<string, MaterialPtr> generatedMaterials;
	vector<vec3> generatedVertices;
	vector<vec2> generatedTexCoords;
	vector<vec3> generatedNormals;
	map<MapKey, unsigned int> indicesMap;
	for (map<string, Material>::iterator i = materials.begin(); i != materials.end(); i++)
	{
		MaterialPtr material(new FireCube::Material);
		material->SetName(i->first);
		material->SetAmbientColor(vec4(i->second.ambientColor.x, i->second.ambientColor.y, i->second.ambientColor.z, 1.0f));
		material->SetDiffuseColor(vec4(i->second.diffuseColor.x, i->second.diffuseColor.y, i->second.diffuseColor.z, 1.0f));
		material->SetSpecularColor(vec4(i->second.specularColor.x, i->second.specularColor.y, i->second.specularColor.z, 1.0f));
		material->SetShininess(i->second.shininess);
		if (!i->second.diffuseTextureName.empty())
		{
			string textureName = i->second.diffuseTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);

			TexturePtr texture = Renderer::GetTextureManager().Create(baseDir + "\\" + textureName);
			if (!texture)
				texture = Renderer::GetTextureManager().Create(textureName);
			material->SetDiffuseTexture(texture);
		}
		if (!i->second.normalTextureName.empty())
		{
			string textureName = i->second.normalTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);

			TexturePtr texture = Renderer::GetTextureManager().Create(baseDir + "\\" + textureName);
			if (!texture)
				texture = Renderer::GetTextureManager().Create(textureName);
			material->SetNormalTexture(texture);
		}
		generatedMaterials[i->first] = material;
	}
	for (map<string, Object>::iterator i = objects.begin(); i != objects.end(); i++)
	{
		NodePtr node(new Node(i->first));
		node->SetParent(root);
		int surfaceNum = 0;
		for (map<string, vector<Face>>::iterator j = i->second.materialFaces.begin(); j != i->second.materialFaces.end(); j++, surfaceNum++)
		{
			ostringstream oss;
			oss << "surface-" << surfaceNum;
			NodePtr surfaceNode(new Node(oss.str()));
			GeometryPtr geometry(new Geometry);
			surfaceNode->SetGeometry(geometry);
			surfaceNode->SetParent(node);
			for (vector<Face>::iterator k = j->second.begin(); k != j->second.end(); k++)
			{
				Face &face = *k;
				FireCube::Face generatedFace;
				for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					MapKey key(face.v[vertexIndex], face.t[vertexIndex], face.n[vertexIndex]);					
					map<MapKey, unsigned int>::iterator keyIter = indicesMap.find(key);
					if (keyIter == indicesMap.end())
					{
						generatedVertices.push_back(vertices[face.v[vertexIndex] - 1]);
						if (face.hasTextureCoordinates)
							generatedTexCoords.push_back(texCoords[face.t[vertexIndex] - 1]);
						if (face.hasNormal)
							generatedNormals.push_back(normals[face.n[vertexIndex] - 1]);

						geometry->GetIndices().push_back(generatedVertices.size() - 1);
						generatedFace.v[vertexIndex] = generatedVertices.size() - 1;

						indicesMap[key] = generatedVertices.size() - 1;
					}
					else
					{
						geometry->GetIndices().push_back(keyIter->second);
						generatedFace.v[vertexIndex] = keyIter->second;
					}
				}
				geometry->GetFaces().push_back(generatedFace);
			}
			geometry->GetVertices() = generatedVertices;
			geometry->GetDiffuseUV() = generatedTexCoords;
			geometry->GetNormals() = generatedNormals;
			geometry->SetMaterial(generatedMaterials[j->first]);
			geometry->SetPrimitiveType(TRIANGLES);
			geometry->SetPrimitiveCount(j->second.size());
			geometry->SetIndexCount(geometry->GetIndices().size());
			if (geometry->GetNormals().empty())
				geometry->CalculateNormals();
			geometry->CalculateTangents();
			geometry->UpdateBuffers();
			geometry->CalculateBoundingBox();
		}
	}
	return root;
}
ObjLoader::Material::Material()
{
	ambientColor.Set(0.0f, 0.0f, 0.0f);
	diffuseColor.Set(1.0f, 1.0f, 1.0f);
	specularColor.Set((0.0f, 0.0f, 0.0f));
	shininess = 100.0f;
}
ObjLoader::MapKey::MapKey()
{

}
ObjLoader::MapKey::MapKey(unsigned int vv, unsigned int vt, unsigned int vn) : v(vv), t(vt), n(vn)
{

}
bool ObjLoader::MapKey::operator < (const MapKey &other) const
{
	if (v < other.v)
		return true;
	else if (v > other.v)
		return false;

	if (t < other.t)
		return true;
	else if (t > other.t)
		return false;

	return n < other.n;
}