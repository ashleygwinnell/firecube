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


class FIRECUBE_API Mesh : public Resource
{
	OBJECT(Mesh)
public:
	Mesh(Engine *engine);

	virtual bool Load(const std::string &filename);
	const std::vector<SharedPtr<Geometry>> &GetGeometries() const;
	const std::vector<SharedPtr<Material>> &GetMaterials() const;
	void AddGeometry(Geometry *geometry, const BoundingBox &boundingBox, Material *material);
	const std::vector<BoundingBox> &GetBoundingBoxes() const;	
	unsigned int GetNumberOfTreeNodes() const;
	SkeletonNode &GetSkeletonRoot();
	std::vector<Animation> &GetAnimations();
	std::vector<std::vector<BoneWeights>> &GetBoneWeights();
	std::vector<std::vector<Bone>> &GetBones();
	BoundingBox GetBoundingBox() const;
private:
	void ReadAnimations(const aiScene *aScene);
	void ReadSkeleton(const aiScene *aScene, const aiNode *aNode, SkeletonNode &node);
	void ProcessAssimpScene(const aiScene *aScene);
	SharedPtr<Material> ProcessAssimpMaterial(const aiMaterial *aMaterial);
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