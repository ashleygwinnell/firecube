#include <sstream>

#include "Mesh.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
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
#include "Rendering/Material.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;

std::vector<std::string> techniqueNames = {
	"NoTexture",
	"DiffuseMap",
	"NormalMap",
	"DiffuseNormalMap",
	"SpecularMap",
	"DiffuseSpecularMap",
	"NormalSpecularMap",
	"DiffuseNormalSpecularMap"
};

Mesh::Mesh(Engine *engine) : Resource(engine)
{
	skeletonRoot.transformation.Identity();
}

bool Mesh::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;
	
	this->filename = filename;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(resolvedFileName,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices | 
		aiProcess_LimitBoneWeights);

	if (!scene)
	{
		LOGERROR("Failed loading model: ", filename);
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

	meshBoneWeights.resize(aScene->mNumMeshes);
	meshBones.resize(aScene->mNumMeshes);
	boundingBoxes.resize(aScene->mNumMeshes);

	for (unsigned int i = 0; i < aScene->mNumMeshes; ++i)
	{
		auto aMesh = aScene->mMeshes[i];
		auto geometry = ProcessAssimpMesh(aMesh, i);
		geometries.push_back(geometry);
		materials.push_back(materialList[aMesh->mMaterialIndex]);
	}

	ReadSkeleton(aScene, aScene->mRootNode, skeletonRoot);

	ReadAnimations(aScene);	

	// Assign each node in the skeleton an index
	unsigned int index = 0;
	BuildTreeIndices(skeletonRoot, index);
	numberOfTreeNodes = CountTreeNodes(skeletonRoot);

	LinkBonesToTree();	
	CalcBoundingBox(skeletonRoot, mat4::IDENTITY);
}

SharedPtr<Material> Mesh::ProcessAssimpMaterial(const aiMaterial *aMaterial)
{
	aiString materialName;
	std::string matName;

	if (aMaterial->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
	{
		matName = materialName.C_Str();
	}
	
	SharedPtr<Material> material = engine->GetResourceCache()->GetResource<Material>("Materials/" + matName + ".xml");
	if (material)
		return material;

	material = new Material(engine);
	material->SetName(matName);

	aiColor3D aColor;
	aiUVTransform uvTransform;
	float value;
	
	if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aColor) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(aColor.r, aColor.g, aColor.b));
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(1.0f));
	}

	if (aMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aColor) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(aColor.r, aColor.g, aColor.b));
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(0.0f));
	}
	
	if (aMaterial->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_SHININESS, value);
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_SHININESS, 15.0f);
	}

	if (aMaterial->Get(AI_MATKEY_OPACITY, value) == AI_SUCCESS)
	{
		material->SetParameter(PARAM_MATERIAL_OPACITY, value);
	}
	else
	{
		material->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
	}

	if (aMaterial->Get(AI_MATKEY_UVTRANSFORM_DIFFUSE(0), uvTransform) == AI_SUCCESS)
	{
		vec3 uOffset(uvTransform.mScaling.x, 0.0f, uvTransform.mTranslation.x);
		vec3 vOffset(0.0f, uvTransform.mScaling.y, uvTransform.mTranslation.y);
		material->SetParameter(PARAM_U_OFFSET, uOffset);
		material->SetParameter(PARAM_V_OFFSET, vOffset);
	}
	
	bool hasDiffuseTexture = ProcessAssimpMaterialTexture(aMaterial, material, aiTextureType_DIFFUSE, TextureUnit::DIFFUSE);
	bool hasNormalTexture = ProcessAssimpMaterialTexture(aMaterial, material, aiTextureType_NORMALS, TextureUnit::NORMAL);
	bool hasSpecularTexture = ProcessAssimpMaterialTexture(aMaterial, material, aiTextureType_SPECULAR, TextureUnit::SPECULAR);
		
	unsigned int techniqueIndex = 0;
	if (hasDiffuseTexture)
	{
		techniqueIndex += 1;
	}
	if (hasNormalTexture)
	{
		techniqueIndex += 2;
	}
	if (hasSpecularTexture)
	{
		techniqueIndex += 4;
	}

	material->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/" + techniqueNames[techniqueIndex] + ".xml"));	

	return material;
}

bool Mesh::ProcessAssimpMaterialTexture(const aiMaterial *aMaterial, Material *material, const aiTextureType &textureType, TextureUnit unit)
{
	aiString file;
	std::string meshBaseDirectory = Filesystem::GetDirectoryName(filename);
	if (aMaterial->GetTexture(textureType, 0, &file) == AI_SUCCESS)
	{		
		std::string resourceName = "Textures/" + Filesystem::GetFileName(file.C_Str());
		if (Filesystem::FindResourceByName(resourceName).empty())
		{
			// Try relative to mesh file
			std::string relativeToMeshFile = meshBaseDirectory + Filesystem::PATH_SEPARATOR + std::string(file.C_Str());
			if (Filesystem::FileExists(relativeToMeshFile))
			{
				material->SetTexture(unit, engine->GetResourceCache()->GetResource<Texture>(relativeToMeshFile));
			}
			else
			{
				// Try as absolute path
				material->SetTexture(unit, engine->GetResourceCache()->GetResource<Texture>(file.C_Str()));
			}
		}
		else
		{
			material->SetTexture(unit, engine->GetResourceCache()->GetResource<Texture>(resourceName));
		}

		return true;
	}

	return false;
}

SharedPtr<Geometry> Mesh::ProcessAssimpMesh(const aiMesh *aMesh, unsigned int meshIndex)
{
	if (aMesh->mNumBones > 0)
	{
		auto &bones = meshBones[meshIndex];
		bones.resize(aMesh->mNumBones);

		auto &boneWeights = meshBoneWeights[meshIndex];
		boneWeights.resize(aMesh->mNumVertices);		

		for (unsigned int i = 0; i < aMesh->mNumBones; ++i)
		{
			auto aBone = aMesh->mBones[i];
			Bone &bone = bones[i];
			bone.name = aBone->mName.C_Str();
										
			bone.offsetMarix = mat4(aBone->mOffsetMatrix.a1, aBone->mOffsetMatrix.b1, aBone->mOffsetMatrix.c1, aBone->mOffsetMatrix.d1,
				aBone->mOffsetMatrix.a2, aBone->mOffsetMatrix.b2, aBone->mOffsetMatrix.c2, aBone->mOffsetMatrix.d2,
				aBone->mOffsetMatrix.a3, aBone->mOffsetMatrix.b3, aBone->mOffsetMatrix.c3, aBone->mOffsetMatrix.d3,
				aBone->mOffsetMatrix.a4, aBone->mOffsetMatrix.b4, aBone->mOffsetMatrix.c4, aBone->mOffsetMatrix.d4);			 


			for (unsigned int j = 0; j < aBone->mNumWeights; ++j)
			{
				unsigned int vertexIndex = aBone->mWeights[j].mVertexId;
				float weight = aBone->mWeights[j].mWeight;
				BoneWeights &bw = boneWeights[vertexIndex];
				unsigned int l = 0;
				while (l < NUM_BONES_PER_VEREX)
				{
					if (bw.boneWeight[l] == 0.0f)
					{
						bw.boneIndex[l] = i;
						bw.boneWeight[l] = weight;
						break;
					}
					++l;
				}

				if (l == NUM_BONES_PER_VEREX)
				{
					LOGWARNING("Number of bones per vertex exceeds maximum of ", NUM_BONES_PER_VEREX);					
				}

				if (weight > 0)
				{
					vec3 vertexBoneSpace = bone.offsetMarix * vec3(aMesh->mVertices[vertexIndex].x, aMesh->mVertices[vertexIndex].y, aMesh->mVertices[vertexIndex].z);
					bone.boundingBox.Expand(vertexBoneSpace);
				}
			}

		}
	}

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
	unsigned int vertexSize = 3 * sizeof(float);
	if (aMesh->HasNormals())
	{
		vertexAttributes |= VertexAttributeType::NORMAL;
		vertexSize += 3 * sizeof(float);
	}
	if (aMesh->HasTextureCoords(0))
	{
		vertexAttributes |= VertexAttributeType::TEXCOORD0;
		vertexSize += 2 * sizeof(float);
	}
	if (aMesh->HasTangentsAndBitangents())
	{
		vertexAttributes |= VertexAttributeType::TANGENT;
		vertexSize += 3 * sizeof(float);
	}

	if (aMesh->mNumBones > 0)
	{
		vertexAttributes |= VertexAttributeType::BONE_WEIGHTS | VertexAttributeType::BONE_INDICES;
		vertexSize += NUM_BONES_PER_VEREX * (sizeof(float) + sizeof(unsigned char));
	}

	std::vector<unsigned char> vertexData(vertexSize * aMesh->mNumVertices);
	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		vec3 position(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);		
		*((float *) &vertexData[i * vertexSize + 0]) = position.x;
		*((float *) &vertexData[i * vertexSize + 4]) = position.y;
		*((float *) &vertexData[i * vertexSize + 8]) = position.z;

		boundingBoxes[meshIndex].Expand(position);

		unsigned int currentOffset = 12;

		if (aMesh->HasNormals())
		{
			vec3 normal(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);			
			
			*((float *)&vertexData[i * vertexSize + currentOffset]) = normal.x;
			*((float *)&vertexData[i * vertexSize + currentOffset + 4]) = normal.y;
			*((float *)&vertexData[i * vertexSize + currentOffset + 8]) = normal.z;
			currentOffset += 12;
		}

		if (aMesh->HasTextureCoords(0))
		{
			*((float *)&vertexData[i * vertexSize + currentOffset]) = aMesh->mTextureCoords[0][i].x;
			*((float *)&vertexData[i * vertexSize + currentOffset + 4]) = aMesh->mTextureCoords[0][i].y;
			currentOffset += 8;
		}

		if (aMesh->HasTangentsAndBitangents())
		{
			vec3 tangent(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z);			
			*((float *)&vertexData[i * vertexSize + currentOffset]) = aMesh->mTangents[i].x;
			*((float *)&vertexData[i * vertexSize + currentOffset + 4]) = aMesh->mTangents[i].y;
			*((float *)&vertexData[i * vertexSize + currentOffset + 8]) = aMesh->mTangents[i].z;
			currentOffset += 12;
		}

		if (aMesh->mNumBones > 0)
		{
			for (unsigned int j = 0; j < NUM_BONES_PER_VEREX; ++j)
			{
				*((float *)&vertexData[i * vertexSize + currentOffset]) = meshBoneWeights[meshIndex][i].boneWeight[j];
				currentOffset += 4;
			}

			for (unsigned int j = 0; j < NUM_BONES_PER_VEREX; ++j)
			{
				*((unsigned char *)&vertexData[i * vertexSize + currentOffset]) = meshBoneWeights[meshIndex][i].boneIndex[j];
				currentOffset += 1;
			}			
		}
	}

	vertexBuffer->LoadData(vertexData.data(), aMesh->mNumVertices, vertexAttributes, BufferType::STATIC);

	geometry->SetPrimitiveType(FireCube::PrimitiveType::TRIANGLES);
	geometry->SetPrimitiveCount(indexData.size() / 3);
	geometry->SetGeometryType(aMesh->mNumBones > 0 ? GeometryType::SKINNED : GeometryType::STATIC);
	geometry->Update();	

	return geometry;
}

const std::vector<SharedPtr<Geometry>> &Mesh::GetGeometries() const
{
	return geometries;
}

const std::vector<SharedPtr<Material>> &Mesh::GetMaterials() const
{
	return materials;
}

void Mesh::AddGeometry(Geometry *geometry, const BoundingBox &boundingBox, Material *material)
{
	geometries.push_back(geometry);
	materials.push_back(material);
	boundingBoxes.push_back(boundingBox);
	this->boundingBox.Expand(boundingBox);
	skeletonRoot.meshes.push_back(geometries.size() - 1);
}

const std::vector<BoundingBox> &Mesh::GetBoundingBoxes() const
{
	return boundingBoxes;
}

void Mesh::ReadSkeleton(const aiScene *aScene, const aiNode *aNode, SkeletonNode &node)
{
	mat4 localTransformation(aNode->mTransformation.a1, aNode->mTransformation.b1, aNode->mTransformation.c1, aNode->mTransformation.d1,
		aNode->mTransformation.a2, aNode->mTransformation.b2, aNode->mTransformation.c2, aNode->mTransformation.d2,
		aNode->mTransformation.a3, aNode->mTransformation.b3, aNode->mTransformation.c3, aNode->mTransformation.d3,
		aNode->mTransformation.a4, aNode->mTransformation.b4, aNode->mTransformation.c4, aNode->mTransformation.d4);
	node.transformation = localTransformation;

	node.name = aNode->mName.C_Str();	
	
	for (unsigned int i = 0; i < aNode->mNumMeshes; ++i)
	{
		node.meshes.push_back(aNode->mMeshes[i]);
	}

	for (unsigned int i = 0; i < aNode->mNumChildren; ++i)
	{
		node.children.push_back(SkeletonNode());
		ReadSkeleton(aScene, aNode->mChildren[i], node.children.back());
	}
}

void Mesh::ReadAnimations(const aiScene *aScene)
{
	for (unsigned int i = 0; i < aScene->mNumAnimations; ++i)
	{
		auto aAnimation = aScene->mAnimations[i];
		animations.push_back(Animation());
		auto &animation = animations.back();
		animation.name = aAnimation->mName.C_Str();
		animation.duration = (float) aAnimation->mDuration;
		animation.ticksPerSecond = (float) aAnimation->mTicksPerSecond;
		for (unsigned int j = 0; j < aAnimation->mNumChannels; ++j)
		{
			auto &aNodeAnim = aAnimation->mChannels[j];
			animation.nodeAnimations.push_back(NodeAnimation());
			auto &nodeAnimation = animation.nodeAnimations.back();
			nodeAnimation.lastPositionIndex = 0;
			nodeAnimation.lastRotationIndex = 0;
			nodeAnimation.lastScaleIndex = 0;
			nodeAnimation.nodeName = aNodeAnim->mNodeName.C_Str();
			for (unsigned int k = 0; k < aNodeAnim->mNumPositionKeys; ++k)
			{
				nodeAnimation.positionAnimation.push_back(std::make_pair((float) aNodeAnim->mPositionKeys[k].mTime, vec3(aNodeAnim->mPositionKeys[k].mValue.x,
					aNodeAnim->mPositionKeys[k].mValue.y, aNodeAnim->mPositionKeys[k].mValue.z)));
			}

			for (unsigned int k = 0; k < aNodeAnim->mNumScalingKeys; ++k)
			{
				nodeAnimation.scaleAnimation.push_back(std::make_pair((float) aNodeAnim->mScalingKeys[k].mTime, vec3(aNodeAnim->mScalingKeys[k].mValue.x,
					aNodeAnim->mScalingKeys[k].mValue.y, aNodeAnim->mScalingKeys[k].mValue.z)));
			}

			for (unsigned int k = 0; k < aNodeAnim->mNumRotationKeys; ++k)
			{
				nodeAnimation.rotationAnimation.push_back(std::make_pair((float) aNodeAnim->mRotationKeys[k].mTime, quat(aNodeAnim->mRotationKeys[k].mValue.x,
					aNodeAnim->mRotationKeys[k].mValue.y, aNodeAnim->mRotationKeys[k].mValue.z, aNodeAnim->mRotationKeys[k].mValue.w)));
			}
		}
	}
}

unsigned int Mesh::GetNodeIndex(SkeletonNode &node, const std::string name)
{
	if (node.name == name)
		return node.nodeIndex;

	for (auto &c : node.children)
	{
		unsigned int childIndex = GetNodeIndex(c, name);
		if (childIndex != std::numeric_limits<unsigned int>::max())
			return childIndex;
	}

	return std::numeric_limits<unsigned int>::max();
}

void Mesh::LinkBonesToTree()
{
	for (auto &bones : meshBones)
	{
		for (auto &bone : bones)
		{
			bone.nodeIndex = GetNodeIndex(skeletonRoot, bone.name);
		}
	}
}

void Mesh::BuildTreeIndices(SkeletonNode &node, unsigned int &index)
{
	node.nodeIndex = index++;
	for (auto &c : node.children)
	{
		BuildTreeIndices(c, index);
	}
}

unsigned int Mesh::CountTreeNodes(SkeletonNode &node)
{
	unsigned int count = 1;
	for (auto &c : node.children)
	{
		count += CountTreeNodes(c);
	}

	return count;
}

unsigned int Mesh::GetNumberOfTreeNodes() const
{
	return numberOfTreeNodes;
}

SkeletonNode &Mesh::GetSkeletonRoot()
{
	return skeletonRoot;
}

std::vector<Animation> &Mesh::GetAnimations()
{
	return animations;
}

std::vector<std::vector<BoneWeights>> &Mesh::GetBoneWeights()
{
	return meshBoneWeights;
}
std::vector<std::vector<Bone>> &Mesh::GetBones()
{
	return meshBones;
}

BoneWeights::BoneWeights()
{
	for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; ++i)
	{
		boneWeight[i] = 0.0f;
		boneIndex[i] = 0;
	}
}

BoundingBox Mesh::GetBoundingBox() const
{
	return boundingBox;
}

void Mesh::CalcBoundingBox(SkeletonNode &node, mat4 transformation)
{
	mat4 modelTransformation = transformation * node.transformation;
	for (auto meshIndex : node.meshes)
	{				
		BoundingBox box = boundingBoxes[meshIndex];
		box.Transform(modelTransformation);
		boundingBox.Expand(box);
	}

	for (auto &c : node.children)
	{
		CalcBoundingBox(c, modelTransformation);
	}
}