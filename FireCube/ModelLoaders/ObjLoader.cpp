#include <fstream>
#include <sstream>

#include "Rendering/Texture.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "ModelLoaders/ObjLoader.h"
#include "Rendering/IndexBuffer.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/RenderingTypes.h"
#include "Math/MathUtils.h"
#include "Rendering/Technique.h"

using namespace FireCube;

ObjLoader::ObjLoader(Engine *engine) : ModelLoader(engine)
{
	objects["default"] = Object();
	currentObject = &objects["default"];
	currentFaces = &objects["default"].facesWithoutMaterial;
}

bool ObjLoader::Load(const std::string &filename, ModelLoadingOptions options)
{
	std::ifstream ifs(filename);
	std::string line;
	baseDir = ExtractDirectory(filename);
	// Parse the file one line at a time
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
	this->options = options;
	return true;
}

void ObjLoader::GenerateScene(Renderer *renderer, Node *root)
{
	//TODO: Implement
}

Node *ObjLoader::GetGeneratedScene()
{
	//TODO: Implement
	return nullptr;
}

const std::vector<Material *> &ObjLoader::GetGeneratedMaterials()
{
	return generatedMaterials;
}


void ObjLoader::ParseVertexLine(const std::string &line)
{	
	std::istringstream iss(line.substr(2));
	vec3 vertex;
	iss >> vertex.x;
	iss >> vertex.y;
	iss >> vertex.z;
	vertices.push_back(vertex);
}

void ObjLoader::ParseTexCoordLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));
	vec2 uv;
	iss >> uv.x;
	iss >> uv.y;
	uv.y = 1.0f - uv.y;
	texCoords.push_back(uv);
}

void ObjLoader::ParseNormalLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));
	vec3 normal;
	iss >> normal.x;
	iss >> normal.y;
	iss >> normal.z;
	normal.Normalize();
	normals.push_back(normal);
}

void ObjLoader::ParseFaceLine(const std::string &line)
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
	i2 = line.find_first_of(' ', i1);
	if (i2 == std::string::npos)
	{
		ParseFaceEntry(line.substr(i1), f.v[2], f.t[2], f.n[2], f.hasTextureCoordinates, f.hasNormal);
		currentFaces->push_back(f);
	}
	else
	{
		ParseFaceEntry(line.substr(i1, i2 - i1), f.v[2], f.t[2], f.n[2], f.hasTextureCoordinates, f.hasNormal);
		currentFaces->push_back(f);

		int i3 = line.find_first_not_of(' ', i2);
		if (i3 != std::string::npos)
		{
			Face f2;
			ParseFaceEntry(line.substr(i3), f2.v[2], f2.t[2], f2.n[2], f2.hasTextureCoordinates, f2.hasNormal);
			f2.v[0] = f.v[0];
			f2.t[0] = f.t[0];
			f2.n[0] = f.n[0];

			f2.v[1] = f.v[2];
			f2.t[1] = f.t[2];
			f2.n[1] = f.n[2];

			currentFaces->push_back(f2);
		}
		
	}
}

void ObjLoader::ParseFaceEntry(const std::string &entry, unsigned int &v, unsigned int &t, unsigned int &n, bool &hasTextureCoordinates, bool &hasNormal)
{
	int rv, rt, rn;
	int i1 = entry.find_first_of('/');
	if (i1 == std::string::npos) // Only vertex index is specified
	{
		std::istringstream iss(entry);
		iss >> rv;
		if (rv < 0)
			v = vertices.size() + rv;
		else
			v = rv;
		hasTextureCoordinates = false;
		hasNormal = false;
		return;
	}

	int i2 = entry.find_first_of('/', i1 + 1);
	if (i2 == std::string::npos) // vertex and texture coordinate
	{
		std::istringstream iss1(entry.substr(0, i1));
		std::istringstream iss2(entry.substr(i1 + 1));
		iss1 >> rv;
		if (rv < 0)
			v = vertices.size() + rv;
		else
			v = rv;
		iss2 >> rt;
		if (rt < 0)
			t = texCoords.size() + rt;
		else
			t = rt;
		hasTextureCoordinates = true;
		hasNormal = false;
		return;
	}
	std::istringstream iss1(entry.substr(0, i1));	
	std::istringstream iss2(entry.substr(i2 + 1));
	iss1 >> rv;
	if (rv < 0)
		v = vertices.size() + rv;
	else
		v = rv;
	iss2 >> rn;
	if (rn < 0)
		n = normals.size() + rn;
	else
		n = rn;
	hasNormal = true;
	if (i2 - i1 - 1 > 0) // vertex texture coordinate and normal
	{
		std::istringstream iss(entry.substr(i1 + 1, i2 - i1 - 1));
		iss >> rt;
		if (rt < 0)
			t = texCoords.size() + rt;
		else
			t = rt;
		hasTextureCoordinates = true;
	}
	else
		hasTextureCoordinates = false;
}

void ObjLoader::ParseObjectLine(const std::string &line)
{
	// Create a new object
	std::string name = line.substr(2);
	objects[name] = Object();
	currentObject = &objects[name];
	if (lastMaterial.empty())
		currentFaces = &objects[name].facesWithoutMaterial;
	else
	{
		// If a previous material was specified all faces from now on will use it
		objects[name].materialFaces[lastMaterial] = std::vector<Face>();
		currentFaces = &objects[name].materialFaces[lastMaterial];
	}

}

void ObjLoader::ParseUseMtlLine(const std::string &line)
{	
	// All faces of current object will now use this material
	lastMaterial = line.substr(7);
	if (currentObject->materialFaces.find(lastMaterial) == currentObject->materialFaces.end())
		currentObject->materialFaces[lastMaterial] = std::vector<Face>();
	currentFaces = &currentObject->materialFaces[lastMaterial];
}

void ObjLoader::ParseMtlLibLine(const std::string &line)
{
	ParseMaterialFile(line.substr(7));	
}

void ObjLoader::ParseMaterialFile(const std::string &filename)
{	
	std::ifstream ifs(baseDir + "\\" + filename);
	std::string line;

	while(getline(ifs, line))
	{		
		line = Trim(line);
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

void ObjLoader::ParseNewMtlLine(const std::string &line)
{
	std::string name = line.substr(7);
	materials[name] = Material();
	currentMaterial = &materials[name];
}

void ObjLoader::ParseAmbientColorLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));	
	iss >> currentMaterial->ambientColor.x;
	iss >> currentMaterial->ambientColor.y;
	iss >> currentMaterial->ambientColor.z;	 
}

void ObjLoader::ParseDiffuseColorLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));	
	iss >> currentMaterial->diffuseColor.x;
	iss >> currentMaterial->diffuseColor.y;
	iss >> currentMaterial->diffuseColor.z;	 
}

void ObjLoader::ParseSpecularColorLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));	
	iss >> currentMaterial->specularColor.x;
	iss >> currentMaterial->specularColor.y;
	iss >> currentMaterial->specularColor.z;	 
}

void ObjLoader::ParseDiffuseTextureMap(const std::string &line)
{
	currentMaterial->diffuseTextureName = line.substr(7);
}

void ObjLoader::ParseNormalTextureMap(const std::string &line)
{
	if (line.substr(0,4) == "bump")
		currentMaterial->normalTextureName = line.substr(5);
	else if (line.substr(0,8) == "map_bump")
		currentMaterial->normalTextureName = line.substr(9);
}

void ObjLoader::ParseShininessLine(const std::string &line)
{
	std::istringstream iss(line.substr(3));
	iss >> currentMaterial->shininess;
}

std::string ObjLoader::ExtractDirectory(const std::string &filename)
{
	int i = filename.find_last_of('\\');
	if (i == std::string::npos)
	{
		i = filename.find_last_of('/');
		if (i == std::string::npos)
		{
			return "";
		}
	}
	return filename.substr(0, i);
}

/*

NodePtr ObjLoader::GenerateSceneGraph()
{
	// Create the root node
	NodePtr root(new Node);
	std::map<std::string, MaterialPtr> generatedMaterials;
	std::vector<vec3> generatedVertices;
	std::vector<vec2> generatedTexCoords;
	std::vector<vec3> generatedNormals;
	std::map<MapKey, unsigned int> indicesMap;

	// Generate materials
	for (std::map<std::string, Material>::iterator i = materials.begin(); i != materials.end(); i++)
	{
		MaterialPtr material(new FireCube::Material);
		material->SetName(i->first);
		material->SetAmbientColor(vec3(i->second.ambientColor.x, i->second.ambientColor.y, i->second.ambientColor.z));
		material->SetDiffuseColor(vec3(i->second.diffuseColor.x, i->second.diffuseColor.y, i->second.diffuseColor.z));
		material->SetSpecularColor(vec3(i->second.specularColor.x, i->second.specularColor.y, i->second.specularColor.z));
		material->SetShininess(i->second.shininess);
		if (!i->second.diffuseTextureName.empty())
		{
			std::string textureName = i->second.diffuseTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);

			TexturePtr texture = Renderer::GetTexturePool().Create(baseDir + "\\" + textureName);
			if (!texture)
				texture = Renderer::GetTexturePool().Create(textureName);
			material->SetDiffuseTexture(texture);
		}
		if (!i->second.normalTextureName.empty())
		{
			std::string textureName = i->second.normalTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);

			TexturePtr texture = Renderer::GetTexturePool().Create(baseDir + "\\" + textureName);
			if (!texture)
				texture = Renderer::GetTexturePool().Create(textureName);
			material->SetNormalTexture(texture);
		}
		generatedMaterials[i->first] = material;
	}

	// Iterate over all objects
	for (std::map<std::string, Object>::iterator i = objects.begin(); i != objects.end(); i++)
	{
		// Create a node for the object
		NodePtr node(new Node(i->first));
		node->SetParent(root);
		int surfaceNum = 0;
		// Iterate over faces for each material
		for (std::map<std::string, std::vector<Face>>::iterator j = i->second.materialFaces.begin(); j != i->second.materialFaces.end(); j++, surfaceNum++)
		{			
			std::ostringstream oss;
			oss << "surface-" << surfaceNum;
			// Create a node and a geometry for each surface
			GeometryNodePtr surfaceNode(new GeometryNode(oss.str()));
			GeometryPtr geometry(new Geometry);
			surfaceNode->SetGeometry(geometry);
			surfaceNode->SetParent(node);
			for (std::vector<Face>::iterator k = j->second.begin(); k != j->second.end(); k++)
			{
				Face &face = *k;
				FireCube::Face generatedFace;
				for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					MapKey key(face.v[vertexIndex], face.t[vertexIndex], face.n[vertexIndex]);					
					// Search this vertex (combination of v/n/t) in the list of vertices
					std::map<MapKey, unsigned int>::iterator keyIter = indicesMap.find(key);
					if (keyIter == indicesMap.end())
					{
						// If not found add it and store it in the map of generated vertices
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
						// If found simply add it's index to the index array
						geometry->GetIndices().push_back(keyIter->second);
						generatedFace.v[vertexIndex] = keyIter->second;
					}
				}
				// Add the generated face to the list of faces
				geometry->GetFaces().push_back(generatedFace);
			}
			geometry->GetVertices() = generatedVertices;
			if (options.flipU || options.flipV)
			{
				for (unsigned int k = 0; k < generatedTexCoords.size(); k++)
				{
					if (options.flipU)
						generatedTexCoords[k].x = 1 - generatedTexCoords[k].x;
					if (options.flipV)
						generatedTexCoords[k].y = 1 - generatedTexCoords[k].y;
				}
			}
			geometry->GetDiffuseUV() = generatedTexCoords;
			geometry->GetNormals() = generatedNormals;
			geometry->SetMaterial(generatedMaterials[j->first]);
			geometry->SetPrimitiveType(TRIANGLES);
			geometry->SetPrimitiveCount(j->second.size());
			geometry->SetVertexCount(geometry->GetIndices().size());
			if (geometry->GetNormals().empty())
				geometry->CalculateNormals();
			geometry->CalculateTangents();
			geometry->UpdateBuffers();
			geometry->CalculateBoundingBox();
		}
	}
	return root;
}

*/

void ObjLoader::GenerateGeometries(Renderer *renderer)
{
	generatedGeometries.clear();
	
	std::map<std::string, FireCube::Material *> allMaterials;
	std::vector<vec3> generatedVertices;
	std::vector<vec2> generatedTexCoords;
	std::vector<vec3> generatedNormals;
	std::map<MapKey, unsigned int> indicesMap;

	// Generate materials
	for (std::map<std::string, Material>::iterator i = materials.begin(); i != materials.end(); i++)
	{
		FireCube::Material *material = new FireCube::Material(engine);
		material->SetName(i->first);
		material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(i->second.ambientColor.x, i->second.ambientColor.y, i->second.ambientColor.z, 1.0f));
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(i->second.diffuseColor.x, i->second.diffuseColor.y, i->second.diffuseColor.z, 1.0f));
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(i->second.specularColor.x, i->second.specularColor.y, i->second.specularColor.z, 1.0f));
		material->SetParameter(PARAM_MATERIAL_SHININESS, i->second.shininess);
		if (!i->second.diffuseTextureName.empty())
		{
			std::string textureName = i->second.diffuseTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);
			
			Texture *texture = engine->GetResourceCache()->GetResource<Texture>(baseDir + "\\" + textureName);
			if (!texture)
				texture = engine->GetResourceCache()->GetResource<Texture>(textureName);
			material->SetTexture(TEXTURE_UNIT_DIFFUSE, texture);
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));
		}
		else
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));

		if (!i->second.normalTextureName.empty())
		{
			std::string textureName = i->second.normalTextureName;
			if (textureName[0] == '/' || textureName[0] == '\\')
				textureName = textureName.substr(1);

			Texture *texture = engine->GetResourceCache()->GetResource<Texture>(baseDir + "\\" + textureName);
			if (!texture)
				texture = engine->GetResourceCache()->GetResource<Texture>(textureName);
			material->SetTexture(TEXTURE_UNIT_NORMAL, texture);
		}
		allMaterials[i->first] = material;
	}

	// Iterate over all objects
	for (std::map<std::string, Object>::iterator i = objects.begin(); i != objects.end(); i++)
	{		
		int surfaceNum = 0;
		// Iterate over faces for each material
		for (std::map<std::string, std::vector<Face>>::iterator j = i->second.materialFaces.begin(); j != i->second.materialFaces.end(); j++, surfaceNum++)
		{			
			Geometry *geometry = new Geometry(renderer);
			VertexBuffer *vertexBuffer = new VertexBuffer(renderer);
			IndexBuffer *indexBuffer = new IndexBuffer(renderer);
			vertexBuffer->SetShadowed(true);
			indexBuffer->SetShadowed(true);
			geometry->SetVertexBuffer(vertexBuffer);
			geometry->SetIndexBuffer(indexBuffer);
			generatedGeometries.push_back(geometry);
			generatedMaterials.push_back(allMaterials[j->first]);
			std::vector<unsigned int> indexData;
			
			for (std::vector<Face>::iterator k = j->second.begin(); k != j->second.end(); k++)
			{
				Face &face = *k;				
				for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					MapKey key(face.v[vertexIndex], face.t[vertexIndex], face.n[vertexIndex]);					
					// Search this vertex (combination of v/n/t) in the list of vertices
					std::map<MapKey, unsigned int>::iterator keyIter = indicesMap.find(key);
					if (keyIter == indicesMap.end())
					{
						// If not found add it and store it in the map of generated vertices
						generatedVertices.push_back(vertices[face.v[vertexIndex] - 1]);
						if (face.hasTextureCoordinates)
							generatedTexCoords.push_back(texCoords[face.t[vertexIndex] - 1]);
						if (face.hasNormal)
							generatedNormals.push_back(normals[face.n[vertexIndex] - 1]);

						indexData.push_back(generatedVertices.size() - 1);						

						indicesMap[key] = generatedVertices.size() - 1;
					}
					else
					{
						// If found simply add it's index to the index array
						indexData.push_back(keyIter->second);						
					}
				}				
			}
			if (generatedNormals.empty())
				MathUtils::CalculateNormals(generatedNormals, generatedVertices, indexData);

			VertexAttributeType vertexAttributes = VertexAttributeType::POSITION | VertexAttributeType::NORMAL;
			unsigned int vertexSize = 6;
			if (generatedTexCoords.empty() == false)
			{
				vertexAttributes |= VertexAttributeType::TEXCOORD0;
				vertexSize += 2;
			}
			std::vector<float> vertexData(vertexSize * generatedVertices.size());
			for (unsigned int k = 0; k < generatedVertices.size(); ++k)
			{
				*((vec3 *) &vertexData[k * vertexSize + 0]) = generatedVertices[k];
				*((vec3 *) &vertexData[k * vertexSize + 3]) = generatedNormals[k];
				if ((vertexAttributes & VertexAttributeType::TEXCOORD0) == VertexAttributeType::TEXCOORD0)
					*((vec2 *) &vertexData[k * vertexSize + 6]) = generatedTexCoords[k];
				boundingBox.Expand(generatedVertices[k]);
			}
			
			vertexBuffer->LoadData(&vertexData[0], generatedVertices.size(), vertexAttributes, STATIC);			
			indexBuffer->LoadData(&indexData[0], indexData.size(), STATIC);

			geometry->SetPrimitiveType(PrimitiveType::TRIANGLES);
			geometry->SetPrimitiveCount(j->second.size());
			geometry->Update();			
		}

		if (i->second.facesWithoutMaterial.empty() == false)
		{
			Geometry *geometry = new Geometry(renderer);
			VertexBuffer *vertexBuffer = new VertexBuffer(renderer);
			IndexBuffer *indexBuffer = new IndexBuffer(renderer);
			vertexBuffer->SetShadowed(true);
			indexBuffer->SetShadowed(true);
			geometry->SetVertexBuffer(vertexBuffer);
			geometry->SetIndexBuffer(indexBuffer);
			generatedGeometries.push_back(geometry);
			std::vector<unsigned int> indexData;
			// Iterate over faces which does not have a material
			for (std::vector<Face>::iterator j = i->second.facesWithoutMaterial.begin(); j != i->second.facesWithoutMaterial.end(); j++, surfaceNum++)
			{
				Face &face = *j;
				for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
				{
					MapKey key(face.v[vertexIndex], face.t[vertexIndex], face.n[vertexIndex]);
					// Search this vertex (combination of v/n/t) in the list of vertices
					std::map<MapKey, unsigned int>::iterator keyIter = indicesMap.find(key);
					if (keyIter == indicesMap.end())
					{
						// If not found add it and store it in the map of generated vertices
						generatedVertices.push_back(vertices[face.v[vertexIndex] - 1]);
						if (face.hasTextureCoordinates)
							generatedTexCoords.push_back(texCoords[face.t[vertexIndex] - 1]);
						if (face.hasNormal)
							generatedNormals.push_back(normals[face.n[vertexIndex] - 1]);

						indexData.push_back(generatedVertices.size() - 1);

						indicesMap[key] = generatedVertices.size() - 1;
					}
					else
					{
						// If found simply add it's index to the index array
						indexData.push_back(keyIter->second);
					}
				}
			}

			if (generatedNormals.empty())
				MathUtils::CalculateNormals(generatedNormals, generatedVertices, indexData);

			VertexAttributeType vertexAttributes = VertexAttributeType::POSITION | VertexAttributeType::NORMAL;
			unsigned int vertexSize = 6;
			if (generatedTexCoords.empty() == false)
			{
				vertexAttributes |= VertexAttributeType::TEXCOORD0;
				vertexSize += 2;
			}
			std::vector<float> vertexData(vertexSize * generatedVertices.size());
			for (unsigned int k = 0; k < generatedVertices.size(); ++k)
			{
				*((vec3 *)&vertexData[k * vertexSize + 0]) = generatedVertices[k];
				*((vec3 *)&vertexData[k * vertexSize + 3]) = generatedNormals[k];
				if ((vertexAttributes & VertexAttributeType::TEXCOORD0) == VertexAttributeType::TEXCOORD0)
					*((vec2 *)&vertexData[k * vertexSize + 6]) = generatedTexCoords[k];
				boundingBox.Expand(generatedVertices[k]);
			}

			vertexBuffer->LoadData(&vertexData[0], generatedVertices.size(), vertexAttributes, STATIC);
			indexBuffer->LoadData(&indexData[0], indexData.size(), STATIC);

			FireCube::Material *material;
			if (allMaterials.find("generatedMaterial") != allMaterials.end())
			{
				material = allMaterials["generatedMaterial"];
			}
			else
			{
				material = new FireCube::Material(engine);
				allMaterials["generatedMaterial"] = material;
			}
			material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(0.0f, 0.0f, 0.0f, 1.0f));
			material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
			material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(0.0f, 0.0f, 0.0f, 1.0f));
			material->SetParameter(PARAM_MATERIAL_SHININESS, 0);
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
			generatedMaterials.push_back(material);
			geometry->SetPrimitiveType(PrimitiveType::TRIANGLES);
			geometry->SetPrimitiveCount(i->second.facesWithoutMaterial.size());
			geometry->Update();
		}
	}
}

const std::vector<Geometry *> &ObjLoader::GetGeneratedGeometries()
{
	return generatedGeometries;
}

BoundingBox ObjLoader::GetBoundingBox() const
{
	return boundingBox;
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