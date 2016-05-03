#pragma once
#include "Utils/utils.h"
#include "Core/Resource.h"
#include "Math/BoundingBox.h"
#include "Rendering/Material.h"
#include "Rendering/VertexBuffer.h"

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;
enum aiTextureType;

namespace FireCube
{

class Engine;
class Geometry;

class SkeletonNode
{
public:
	std::string name;
	mat4 transformation;
	unsigned int nodeIndex;
	
	bool hasNodeAnim;
	unsigned int nodeAnimIndex;
	
	std::vector<unsigned int> meshes;
	std::vector<SkeletonNode> children;
};

class NodeAnimation
{
public:
	std::string nodeName;
	std::vector<std::pair<float, vec3>> positionAnimation;
	std::vector<std::pair<float, vec3>> scaleAnimation;
	std::vector<std::pair<float, quat>> rotationAnimation;
	unsigned int lastPositionIndex;
	unsigned int lastScaleIndex;
	unsigned int lastRotationIndex;
};

class Animation
{
public:
	std::string name;
	std::vector<NodeAnimation> nodeAnimations;
	float duration;
	float ticksPerSecond;
};

class BoneWeights
{
public:
	BoneWeights();
	unsigned int boneIndex[NUM_BONES_PER_VEREX];
	float boneWeight[NUM_BONES_PER_VEREX];
};

class Bone
{
public:
	std::string name;
	mat4 offsetMarix;
	unsigned int nodeIndex;
	BoundingBox boundingBox;
};

/**
* A mesh resource. This class stores geometries, materials, bone skeleton and animations  
*/
class FIRECUBE_API Mesh : public Resource
{
	FIRECUBE_OBJECT(Mesh)
public:
	Mesh(Engine *engine);

	/**
	* Loads a mesh
	* @param filename The path to the mesh file
	*/
	virtual bool Load(const std::string &filename);
	
	/**
	* @returns The geometries of this mesh
	*/
	const std::vector<SharedPtr<Geometry>> &GetGeometries() const;
	
	/**
	* @returns The materials of this mesh. Note that this list might have duplicates as it contains a material per geometry
	*/
	const std::vector<SharedPtr<Material>> &GetMaterials() const;
	
	/**
	* Adds a geometry to this mesh
	* @param geometry The geometry to add
	* @param boundingBox The bounding box of this geometry
	* @param material The material of this geometry
	*/
	void AddGeometry(Geometry *geometry, const BoundingBox &boundingBox, Material *material);
	
	/**
	* @returns The bounding boxes of this mesh (one bounding box per geometry)
	*/
	const std::vector<BoundingBox> &GetBoundingBoxes() const;
	
	/**
	* @returns The number of nodes in the skeleton
	*/	
	unsigned int GetNumberOfTreeNodes() const;
	
	/**
	* @returns The skeleton's root
	*/
	SkeletonNode &GetSkeletonRoot();
	
	/**
	* @reutrns The animations of this mesh
	*/
	std::vector<Animation> &GetAnimations();
	
	/**
	* @returns the bone weights of this mesh. A list of bone weights per geometry 
	*/
	std::vector<std::vector<BoneWeights>> &GetBoneWeights();
	
	/**
	* @returns the bones of this mesh. A list of bones per geometry 
	*/
	std::vector<std::vector<Bone>> &GetBones();
	
	/**
	* @returns The bounding box of this mesh
	*/
	BoundingBox GetBoundingBox() const;
private:
	void ReadAnimations(const aiScene *aScene);
	void ReadSkeleton(const aiScene *aScene, const aiNode *aNode, SkeletonNode &node);
	void ProcessAssimpScene(const aiScene *aScene);
	SharedPtr<Material> ProcessAssimpMaterial(const aiMaterial *aMaterial);
	bool ProcessAssimpMaterialTexture(const aiMaterial *aMaterial, Material *material, const aiTextureType &textureType, TextureUnit unit);
	SharedPtr<Geometry> ProcessAssimpMesh(const aiMesh *aMesh, unsigned int meshIndex);
	unsigned int GetNodeIndex(SkeletonNode &node, const std::string name);
	void LinkBonesToTree();
	void BuildTreeIndices(SkeletonNode &node, unsigned int &index);
	unsigned int CountTreeNodes(SkeletonNode &node);	
	void CalcBoundingBox(SkeletonNode &node, mat4 transformation);

	std::vector<BoundingBox> boundingBoxes;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;
	SkeletonNode skeletonRoot;
	
	std::vector<std::vector<BoneWeights>> meshBoneWeights;
	std::vector<std::vector<Bone>> meshBones;
	std::vector<Animation> animations;
	unsigned int numberOfTreeNodes;
	
	BoundingBox boundingBox;
};

}