#pragma once

#include "Scene/Renderable.h"
#include "Geometry/Mesh.h"

namespace FireCube
{

class Mesh;

class AnimatedModel : public Renderable
{
	FIRECUBE_OBJECT(AnimatedModel)
public:
	AnimatedModel(Engine *engine);
	AnimatedModel(Engine *engine, Mesh *mesh);
	void CreateFromMesh(Mesh *mesh);
	virtual void IntersectRay(RayQuery &rayQuery);
	virtual void UpdateRenderableParts();
	void Update(float time);
	void SetAnimation(unsigned int index);
	virtual Component *Clone() const;
protected:

	AnimatedModel(const AnimatedModel &other);

	void CreateRenderableParts(SkeletonNode &skeletonNode, std::vector<RenderablePart> &nonSkinned, std::vector<unsigned int> &nonSkinnedGeometryIndex, std::vector<RenderablePart> &skinned, std::vector<unsigned int> &skinnedGeometryIndex);
	void CalculateNodeAnimations(float animationTime);
	virtual void UpdateWorldBoundingBox();	

	void BuildTreeTransformations(SkeletonNode &node, mat4 parentTransform);
	
	void UpdateTreeNodeAnims(SkeletonNode &node);
	void UpdateSkinningMatrices();	

	unsigned int FindPosition(float animationTime, const NodeAnimation &nodeAnim);
	unsigned int FindRotation(float animationTime, const NodeAnimation &nodeAnim);
	unsigned int FindScaling(float animationTime, const NodeAnimation &nodeAnim);
	void CalcInterpolatedPosition(vec3 &out, float animationTime, NodeAnimation &nodeAnim);
	void CalcInterpolatedRotation(quat &out, float animationTime, NodeAnimation &nodeAnim);
	void CalcInterpolatedScaling(vec3 &out, float animationTime, NodeAnimation &nodeAnim);

	std::vector<BoundingBox> boundingBoxes;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;
	SkeletonNode skeletonRoot;	
	std::vector<std::vector<BoneWeights>> meshBoneWeights;
	std::vector<std::vector<Bone>> meshBones;
	std::vector<Animation> animations;
	std::vector<mat4> nodeAnimTransformations;
	std::vector<std::vector<mat4>> skinMatrices;
	float currentTime;
	float prevTime;
	unsigned int currentAnimation;

	std::vector<mat4> nodeTransformations;
	std::vector<unsigned int> nonSkinnedRenderablePartsNodeIndices;	
	unsigned int numNonSkinnedRenderableParts;
	std::vector<unsigned int> renderablePartGeometryIndex;
};

}