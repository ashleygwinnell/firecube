#include <sstream>

#include "Mesh.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Geometry/ModelLoader.h"
#include "ModelLoaders/ColladaLoader.h"
#include "ModelLoaders/m3dsLoader.h"
#include "ModelLoaders/ObjLoader.h"
#include "Core/Engine.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "assimp/Importer.hpp" // C++ importer interface
#include "assimp/scene.h" // Output data structure
#include "assimp/postprocess.h" // Post processing flags
#include "Rendering/Texture.h"
#include "Rendering/Technique.h"
#include "Core/ResourceCache.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;

Mesh::Mesh(Engine *engine) : Resource(engine)
{

}

bool Mesh::LoadOld(const std::string &filename)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return false;
	std::string::size_type d;
	d = filename.find_last_of(".");		
	LOGINFO("Created model with name:", filename);
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		ModelLoader *modelLoader = nullptr;
		if (ext == "3ds")
			modelLoader = new M3dsLoader(engine);		
		else if (ext == "dae")		
			modelLoader = new ColladaLoader(engine);
		else if (ext == "obj")
			modelLoader = new ObjLoader(engine);

		if (modelLoader && modelLoader->Load(file))
		{
			modelLoader->GenerateGeometries(engine->GetRenderer());						
			for (unsigned int i = 0; i < modelLoader->GetGeneratedGeometries().size(); ++i)
			{
				geometries.push_back(SharedPtr<Geometry>(modelLoader->GetGeneratedGeometries()[i]));
				materials.push_back(SharedPtr<Material>(modelLoader->GetGeneratedMaterials()[i]));
			}
				
			boundingBox = modelLoader->GetBoundingBox();

			delete modelLoader;
			return true;
		}		
	}
	return false;
}

bool Mesh::Load(const std::string &filename)
{	
	Assimp::Importer importer;
	
	const aiScene* scene = importer.ReadFile(filename,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
	
	if (!scene)
	{
		//DoTheErrorLogging(importer.GetErrorString());
		return false;
	}
	
	ProcessAssimpScene(scene);
	
	return true;
}

void Mesh::ProcessAssimpScene(const aiScene *aScene)
{
	std::vector<SharedPtr<Material>> materialList;
	for (unsigned int i = 0; i < aScene->mNumMaterials; ++i)
	{
		auto aMaterial = aScene->mMaterials[i];
		auto material = ProcessAssimpMaterial(aMaterial);
		materialList.push_back(material);
	}

	/*for (unsigned int i = 0; i < aScene->mNumMeshes; ++i)
	{
		auto aMesh = aScene->mMeshes[i];
		auto geometry = ProcessAssimpMesh(aMesh);
		geometries.push_back(geometry);
		materials.push_back(materialList[aMesh->mMaterialIndex]);
	}*/

	ProcessAssimpNode(aScene, aScene->mRootNode, materialList, mat4::IDENTITY);
}

SharedPtr<Material> Mesh::ProcessAssimpMaterial(const aiMaterial *aMaterial)
{	
	SharedPtr<Material> material = new Material(engine);
	
	aiColor3D aColor;	
	float value;
	aiString file;

	if (aMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aColor) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(aColor.r, aColor.g, aColor.b, 1.0f));
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_AMBIENT, vec4::ZERO);
	}

	if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aColor) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(aColor.r, aColor.g, aColor.b, 1.0f));
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4::ZERO);
	}

	if (aMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aColor) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(aColor.r, aColor.g, aColor.b, 1.0f));
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec4::ZERO);
	}

	if (aMaterial->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_SHININESS, value);
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_SHININESS, 1.0f);
	}

	bool hasDiffuseTexture = false;
	bool hasNormalTexture = false;
	if (aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
	{
		hasDiffuseTexture = true;
		material->SetTexture(TextureUnit::DIFFUSE, engine->GetResourceCache()->GetResource<Texture>(file.C_Str()));
	}

	if (aMaterial->GetTexture(aiTextureType_NORMALS, 0, &file) == AI_SUCCESS)
	{
		hasNormalTexture = true;
		material->SetTexture(TextureUnit::NORMAL, engine->GetResourceCache()->GetResource<Texture>(file.C_Str()));
	}

	if (hasDiffuseTexture && hasNormalTexture)
	{
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseNormalMap.xml"));
	}
	else if (hasDiffuseTexture)
	{
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));
	}
	else if (hasNormalTexture)
	{
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NormalMap.xml"));
	}
	else
	{
		material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
	}

	return material;
}

SharedPtr<Geometry> Mesh::ProcessAssimpMesh(const aiMesh *aMesh, mat4 transformation)
{
	SharedPtr<Geometry> geometry = new Geometry(engine->GetRenderer());
	auto vertexBuffer = new VertexBuffer(engine->GetRenderer());
	auto indexBuffer = new IndexBuffer(engine->GetRenderer());

	geometry->SetVertexBuffer(vertexBuffer);
	geometry->SetIndexBuffer(indexBuffer);

	vertexBuffer->SetShadowed(true);
	indexBuffer->SetShadowed(true);
	
	std::vector<unsigned int> indexData;
	for (unsigned int i = 0; i < aMesh->mNumFaces; ++i)
	{
		auto &aFace = aMesh->mFaces[i];
		if (aFace.mNumIndices == 3)
		{
			indexData.push_back(aFace.mIndices[0]);
			indexData.push_back(aFace.mIndices[1]);
			indexData.push_back(aFace.mIndices[2]);
		}
	}

	indexBuffer->LoadData(indexData.data(), indexData.size(), BufferType::STATIC);

	VertexAttributeType vertexAttributes = VertexAttributeType::POSITION;
	unsigned int vertexSize = 3;
	if (aMesh->HasNormals())
	{
		vertexAttributes |= VertexAttributeType::NORMAL;
		vertexSize += 3;
	}
	if (aMesh->HasTextureCoords(0))
	{
		vertexAttributes |= VertexAttributeType::TEXCOORD0;
		vertexSize += 2;
	}
	if (aMesh->HasTangentsAndBitangents())
	{
		vertexAttributes |= VertexAttributeType::TANGENT;
		vertexSize += 3;
	}

	std::vector<float> vertexData(vertexSize * aMesh->mNumVertices);
	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		vec3 position(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
		position = transformation * position;
		vertexData[i * vertexSize + 0] = position.x;
		vertexData[i * vertexSize + 1] = position.y;
		vertexData[i * vertexSize + 2] = position.z;

		boundingBox.Expand(position);

		unsigned int currentOffset = 3;

		if (aMesh->HasNormals())
		{
			vec3 normal(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
			normal = normal.TransformNormal(transformation);
			vertexData[i * vertexSize + currentOffset++] = normal.x;
			vertexData[i * vertexSize + currentOffset++] = normal.y;
			vertexData[i * vertexSize + currentOffset++] = normal.z;
		}

		if (aMesh->HasTextureCoords(0))
		{
			vertexData[i * vertexSize + currentOffset++] = aMesh->mTextureCoords[0][i].x;
			vertexData[i * vertexSize + currentOffset++] = 1.0f - aMesh->mTextureCoords[0][i].y;			
		}

		if (aMesh->HasTangentsAndBitangents())
		{
			vec3 tangent(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z);
			tangent = tangent.TransformNormal(transformation);
			vertexData[i * vertexSize + currentOffset++] = aMesh->mTangents[i].x;
			vertexData[i * vertexSize + currentOffset++] = aMesh->mTangents[i].y;
			vertexData[i * vertexSize + currentOffset++] = aMesh->mTangents[i].z;
		}
	}

	vertexBuffer->LoadData(vertexData.data(), aMesh->mNumVertices, vertexAttributes, BufferType::STATIC);

	geometry->SetPrimitiveType(FireCube::PrimitiveType::TRIANGLES);
	geometry->SetPrimitiveCount(indexData.size() / 3);
	geometry->Update();

	return geometry;
}

void Mesh::ProcessAssimpNode(const aiScene *aScene, const aiNode *aNode, const std::vector<SharedPtr<Material>> &materialList, mat4 parentTransformation)
{
	mat4 localTransformation(aNode->mTransformation.a1, aNode->mTransformation.b1, aNode->mTransformation.c1, aNode->mTransformation.d1,
							 aNode->mTransformation.a2, aNode->mTransformation.b2, aNode->mTransformation.c2, aNode->mTransformation.d2,
							 aNode->mTransformation.a3, aNode->mTransformation.b3, aNode->mTransformation.c3, aNode->mTransformation.d3,
							 aNode->mTransformation.a4, aNode->mTransformation.b4, aNode->mTransformation.c4, aNode->mTransformation.d4);	

	mat4 transformation = parentTransformation * localTransformation;

	for (unsigned int i = 0; i < aNode->mNumMeshes; ++i)
	{
		auto aMesh = aScene->mMeshes[aNode->mMeshes[i]];
		auto geometry = ProcessAssimpMesh(aMesh, transformation);
		geometries.push_back(geometry);
		materials.push_back(materialList[aMesh->mMaterialIndex]);
	}

	for (unsigned int i = 0; i < aNode->mNumChildren; ++i)
	{
		ProcessAssimpNode(aScene, aNode->mChildren[i], materialList, transformation);
	}
}

const std::vector<SharedPtr<Geometry>> &Mesh::GetGeometries() const
{
	return geometries;
}

const std::vector<SharedPtr<Material>> &Mesh::GetMaterials() const
{
	return materials;
}

void Mesh::AddGeometry(Geometry *geometry, Material *material)
{
	geometries.push_back(geometry);
	materials.push_back(material);
}

const BoundingBox &Mesh::GetBoundingBox() const
{
	return boundingBox;
}

void Mesh::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
}