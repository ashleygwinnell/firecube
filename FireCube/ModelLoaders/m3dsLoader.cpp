#include <fstream>
#include <sstream>

#include "Utils/Logger.h"
#include "Rendering/Texture.h"
#include "Geometry/Material.h"
#include "ModelLoaders/m3dsLoader.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Renderer.h"
#include "Rendering/Technique.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Math/MathUtils.h"
#include "Scene/Node.h"
#include "Rendering/StaticModel.h"

using namespace FireCube;

M3dsLoader::M3dsLoader(Engine *engine) : ModelLoader(engine)
{

}

M3dsLoader::~M3dsLoader()
{
	for (auto material : materials)
		delete material;
}

bool M3dsLoader::Load(const std::string &filename, ModelLoadingOptions options)
{
	std::ifstream f(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!f.is_open())
		return false;

	// Allocate a buffer to hold the entire file
	unsigned int l = (unsigned int)f.tellg();
	buffer.resize(l);
	f.seekg(0, std::ios_base::beg);
	f.read(&buffer[0], l);
	curPos = &buffer[0];

	// Read the main chunk
	ReadMainChunk();

	// Link between material names of sub meshes and the actual read materials
	std::vector<std::pair<std::pair<unsigned int, unsigned int>, std::string>>::iterator meshMat;
	for (meshMat = meshMaterial.begin(); meshMat != meshMaterial.end(); meshMat++)
		object[meshMat->first.first].mesh[meshMat->first.second].material = GetMaterialByName(meshMat->second);
	std::vector<std::pair<unsigned int, mat4>>::iterator objMatrix;

	// Search for objects without sub meshes, create a default material
	// and assign all faces to it
	for (unsigned int k = 0; k < object.size(); k++)
	{		 
		if (object[k].mesh.size() == 0)
		{
			Material *mat = new Material;
			mat->ambientColor = vec3(0, 0, 0);
			mat->diffuseColor = vec3(1, 1, 1);
			mat->specularColor = vec3(0, 0, 0);			
			materials.push_back(mat);
			object[k].mesh.push_back(Mesh());
			Mesh &mesh = object[k].mesh.back();
			mesh.material = mat;
			mesh.indices = object[k].indices;
		}
	}
	this->options = options;
	return true;
}


Node *M3dsLoader::GetGeneratedScene()
{
	//TODO: Implement
	return nullptr;
}

const std::vector<FireCube::Material *> &M3dsLoader::GetGeneratedMaterials()
{
	return generatedMaterials;
}

/*

NodePtr M3dsLoader::GenerateSceneGraph()
{
	// Create the root node
	NodePtr ret(new Node);
	for (unsigned int i = 0; i < object.size(); i++)
	{   
		// Create a node for each object
		NodePtr objectNode(new Node(object[i].name));
		ret->AddChild(objectNode);
		for (unsigned int j = 0; j < object[i].mesh.size(); j++)
		{
			// Create a geometry for each sub mesh
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
			geom->CopyFacesToIndices();
			geom->SetPrimitiveType(TRIANGLES);
			geom->SetPrimitiveCount(geom->GetFaces().size());
			geom->SetVertexCount(geom->GetFaces().size() * 3);
			geom->CalculateNormals();
			geom->CalculateTangents();
			geom->UpdateBuffers();
			geom->CalculateBoundingBox();
			std::ostringstream oss;
			oss << object[i].name << "-surface-" << j;
			// Create a node for the sub mesh
			GeometryNodePtr node(new GeometryNode(oss.str()));
			node->SetGeometry(geom);
			objectNode->AddChild(node);
		}        
	}    
	return ret;
}

*/

VertexBuffer *M3dsLoader::CreateVertexBufferAndBoundingBoxOfObject(Object &object, BoundingBox &boundingBox)
{
	MathUtils::CalculateNormals(object.normal, object.vertex, object.indices);		
	if (!object.uv.empty())
		MathUtils::CalculateTangents(object.vertex, object.normal, object.uv, object.indices, object.tangents);		
	VertexBuffer *vertexBuffer = new VertexBuffer(engine->GetRenderer());
	vertexBuffer->SetShadowed(true);
	unsigned int vertexSize = 3 + 3;
	unsigned int vertexAttributes = VERTEX_ATTRIBUTE_POSITION | VERTEX_ATTRIBUTE_NORMAL;		
	if (!object.uv.empty())
	{
		vertexAttributes |= VERTEX_ATTRIBUTE_TEXCOORD0 | VERTEX_ATTRIBUTE_TANGENT;
		vertexSize += 2 + 3;
	}
	std::vector<float> vertexData(vertexSize * object.vertex.size());		
	for (unsigned int j = 0; j < object.vertex.size(); ++j)
	{
		*((vec3 *) &vertexData[j * vertexSize + 0]) = object.vertex[j];
		*((vec3 *) &vertexData[j * vertexSize + 3]) = object.normal[j];
		if (vertexAttributes & VERTEX_ATTRIBUTE_TEXCOORD0)
		{

			vec2 uv = object.uv[j];
			if (options.flipU)
				uv.x = 1.0f - uv.x;
			if (options.flipV)
				uv.y = 1.0f - uv.y;
			*((vec2 *) &vertexData[j * vertexSize + 6]) = uv;
		}
		if (vertexAttributes & VERTEX_ATTRIBUTE_TANGENT)
		{				
			*((vec3 *) &vertexData[j * vertexSize + 8]) = object.tangents[j];
		}

		boundingBox.Expand(object.vertex[j]);
	}		
	vertexBuffer->LoadData(&vertexData[0], object.vertex.size(), vertexAttributes, STATIC);
	return vertexBuffer;
}
Geometry *M3dsLoader::CreateGeometryOfMesh(Mesh &mesh, VertexBuffer *vertexBuffer)
{
	Geometry *geom = new Geometry(engine->GetRenderer());
	IndexBuffer *indexBuffer = new IndexBuffer(engine->GetRenderer());
	indexBuffer->SetShadowed(true);
	geom->SetVertexBuffer(vertexBuffer);
	geom->SetIndexBuffer(indexBuffer);
						
	indexBuffer->LoadData(&mesh.indices[0], mesh.indices.size(), STATIC);

	geom->SetPrimitiveType(TRIANGLES);
	geom->SetPrimitiveCount(mesh.indices.size() / 3);
	geom->Update();		
	return geom;
}

FireCube::Material *M3dsLoader::CreateMaterialOfMesh(Mesh &mesh)
{	
	FireCube::Material *material;
	if (materialsMap.find(mesh.material) != materialsMap.end())
	{
		material = materialsMap[mesh.material];
	}
	else
	{
		material = new FireCube::Material(engine);		
		material->SetName(mesh.material->name);
		material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(mesh.material->ambientColor, 1.0f));
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(mesh.material->diffuseColor, 1.0f));
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(mesh.material->specularColor, 1.0f));
		material->SetParameter(PARAM_MATERIAL_SHININESS, mesh.material->shininess);

		if (mesh.material->diffuseTextureMap.empty() == false)
		{
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));
			material->SetTexture(TEXTURE_UNIT_DIFFUSE, engine->GetResourceCache()->GetResource<Texture>(mesh.material->diffuseTextureMap));
		}
		else
			material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
		materialsMap[mesh.material] = material;
	}
	
	return material;
}

void M3dsLoader::GenerateGeometries(Renderer *renderer)
{
	generatedGeometries.clear();
	for (unsigned int i = 0; i < object.size(); i++)
	{   
		if (object[i].vertex.empty())
			continue;
		VertexBuffer *vertexBuffer = CreateVertexBufferAndBoundingBoxOfObject(object[i], boundingBox);
		//NodePtr objectNode(new Node(object[i].name));		
		for (unsigned int j = 0; j < object[i].mesh.size(); j++)
		{
			// Create a geometry for each sub mesh
			Geometry *geom = CreateGeometryOfMesh(object[i].mesh[j], vertexBuffer);
			generatedGeometries.push_back(geom);
			generatedMaterials.push_back(CreateMaterialOfMesh(object[i].mesh[j]));
		}        
	}    	
}

void M3dsLoader::GenerateScene(Renderer *renderer, Node *root)
{	
	for (unsigned int i = 0; i < object.size(); i++)
	{   
		if (object[i].vertex.empty())
			continue;
		Node *objectNode = root->CreateChild(object[i].name);
		VertexBuffer *vertexBuffer = CreateVertexBufferAndBoundingBoxOfObject(object[i], boundingBox);		
		for (unsigned int j = 0; j < object[i].mesh.size(); j++)
		{
			// Create a geometry for each sub mesh
			StaticModel *staticModel = objectNode->CreateComponent<StaticModel>();			
			Geometry *geom = CreateGeometryOfMesh(object[i].mesh[j], vertexBuffer);
			FireCube::Material *material = CreateMaterialOfMesh(object[i].mesh[j]);
			staticModel->AddRenderablePart(geom, material);
			staticModel->SetBoundingBox(boundingBox); // TODO: Calculate bounding box for each mesh
			generatedGeometries.push_back(geom);
		}        
	}    	
}

const std::vector<Geometry *> &M3dsLoader::GetGeneratedGeometries()
{
	return generatedGeometries;
}

BoundingBox M3dsLoader::GetBoundingBox() const
{
	return boundingBox;
}

void M3dsLoader::ReadMainChunk()
{
	// Read the chunk id and length
	unsigned short int id = *(unsigned short int*)curPos;
	unsigned int len = *(unsigned int*)(curPos + 2) - 6;
	curPos += 6;
	char *startPos = curPos;
	if (id != MAIN3DS)
		return;
	
	// Read all child chunks
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
	std::string name = curPos;
	curPos += name.size() + 1;

	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		
		// A tri mesh chunk declares a new object
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
			ReadVerticesListChunk(); // Read vertices		
		else if (subId == TRI_FACELIST)
			ReadFacesListChunk(subLen); // Read faces		
		else if (subId == TRI_TEXCOORDLIST)
			ReadTexCoordListChunk(); // Read texture coordinates
		else if (subId == TRI_MATRIX)
			ReadObjectMatrixChunk(); // Read the transformation matrix
		else
			curPos += subLen;
	}
}

void M3dsLoader::ReadVerticesListChunk()
{
	// Read the vertices of the current object
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
	// Read the face list of the current object
	Object &obj = object.back();
	char *startPos = curPos;
	obj.indices.resize((*(unsigned short int*)curPos) * 3);
	curPos += 2;
	for (unsigned int i = 0; i < obj.indices.size(); i += 3)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			obj.indices[i + j] = *(unsigned short int*)curPos;
			curPos += 2;
		}
		curPos += 2;
	}
	
	// Read the face list of each material.
	// This list contains indices to the above list which describes the faces using a specific material.
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
	// Read the texture coordinates of the current object
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
	// Create a new sub mesh of this object
	Object &obj = object.back();
	// Read the material name
	std::string matName = curPos;
	curPos += matName.size() + 1;
	obj.mesh.push_back(Mesh());
	Mesh &mesh = obj.mesh.back();
	// Create an association between (object id, sub mesh id) -> material name.
	// Used because the actual definitions of the materials might appear only later in the file.
	meshMaterial.push_back(std::make_pair(std::make_pair(object.size() - 1, obj.mesh.size() - 1), matName));
	unsigned short int count = *(unsigned short int*)curPos;
	curPos += 2;
	// Copy the faces from the object to the faces of this sub mesh
	// using the indices specified
	for (unsigned int i = 0; i < count; i++)
	{
		mesh.indices.push_back(obj.indices[(*(unsigned short int*)curPos) * 3 + 0]);
		mesh.indices.push_back(obj.indices[(*(unsigned short int*)curPos) * 3 + 1]);
		mesh.indices.push_back(obj.indices[(*(unsigned short int*)curPos) * 3 + 2]);
		curPos += 2;
	}
}

void M3dsLoader::ReadObjectMatrixChunk()
{
	// Read the transformation of the current object
	float *arr = (float*)curPos;
	mat4 mat = mat4::IDENTITY;
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
	objectMatrix.push_back(std::make_pair(object.size() - 1, mat));
	curPos += 4 * 12;
}

void M3dsLoader::ReadMaterialListChunk(unsigned int length)
{
	// Read the list of materials
	char *startPos = curPos;

	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == MAT_NAME)
			ReadMaterialNameChunk(); // Read an create a new material
		else if (subId == MAT_AMBIENT)
			curMaterial->ambientColor = ReadMaterialColorChunk(subLen); // Read ambient color of the current material
		else if (subId == MAT_DIFFUSE)
			curMaterial->diffuseColor = ReadMaterialColorChunk(subLen); // Read diffuse color of the current material
		else if (subId == MAT_SPECULAR)
			curMaterial->specularColor = ReadMaterialColorChunk(subLen); // Read specular color of the current material
		else if (subId == MAT_TEXMAP)
			curMaterial->diffuseTextureMap = ReadMaterialTexMapChunk(subLen); // Read diffuse texture of the current material
		else if (subId == MAT_SHININESS)
			curMaterial->shininess = ReadMaterialShininessChunk(subLen); // Read shininess of the current material
		else
			curPos += subLen;
	}
}

void M3dsLoader::ReadMaterialNameChunk()
{
	curMaterial = new Material;
	materials.push_back(curMaterial);
	curMaterial->name = curPos;
	curPos += curMaterial->name.size() + 1;
}

vec3 M3dsLoader::ReadColorFChunk()
{
	// Read a floating point color chunk
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
	// Read a byte color chunk
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
	// Read a material color chunk
	// Can contain a float or byte color chunk
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
	// Read the material's shininess value
	// Can contain a float or a byte percentage chunk
	float ret = 0.0f;
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

std::string M3dsLoader::ReadMapNameChunk()
{
	// Read the file name of a texture map
	std::string ret = curPos;
	curPos += ret.size() + 1;
	return ret;
}

std::string M3dsLoader::ReadMaterialTexMapChunk(unsigned int length)
{
	std::string ret;
	char *startPos = curPos;
	while ((unsigned int)(curPos - startPos) < length)
	{
		unsigned short int subId = *(unsigned short int*)curPos;
		unsigned int subLen = *(unsigned int*)(curPos + 2) - 6;
		curPos += 6;
		if (subId == MAT_MAPNAME)
			ret = ReadMapNameChunk(); // Read an load the texture file name
		else
			curPos += subLen;
	}
	return ret;
}

M3dsLoader::Material *M3dsLoader::GetMaterialByName(const std::string &name)
{	
	// Search for a material with the given name
	for (auto i = materials.begin(); i != materials.end(); ++i)
	{
		if ((*i)->name == name)
			return *i;
	}
	return nullptr;
}

float M3dsLoader::ReadPercentageBChunk()
{
	// Read an integer percentage chunk
	float ret = *(unsigned short int*)curPos;
	curPos += 2;
	return ret;
}

float M3dsLoader::ReadPercentageFChunk()
{
	// Read a floating point percentage chunk
	float ret = *(float*)curPos;
	curPos += 4;
	return ret;
}