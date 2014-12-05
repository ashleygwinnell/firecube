#pragma once

#include "Renderable.h"
#include "Geometry/AnimatedMesh.h"

namespace FireCube
{

class AnimatedMesh;

class AnimatedModel : public Renderable
{
	OBJECT(AnimatedModel)
public:
	AnimatedModel(Engine *engine);
	AnimatedModel(Engine *engine, AnimatedMesh *mesh);
	void CreateFromMesh(AnimatedMesh *mesh);
	virtual void IntersectRay(RayQuery &rayQuery);
	virtual void UpdateRenderableParts();
	void Update(float time);
	void SetAnimation(unsigned int index);
protected:
	void CreateRenderableParts(SkeletonNode &skeletonNode, std::vector<RenderablePart> &nonSkinned, std::vector<RenderablePart> &skinned);		
	void CalculateNodeAnimations(float animationTime);
	virtual void UpdateWorldBoundingBox();	

	void BuildTreeTransformations(SkeletonNode &node, mat4 parentTransform);
	
	void UpdateTreeNodeAnims(SkeletonNode &node);
	void UpdateSkinningMatrices();	

	unsigned int FindPosition(float animationTime, const NodeAnimation &nodeAnim);
	unsigned int FindRotation(float animationTime, const NodeAnimation &nodeAnim);
	unsigned int FindScaling(float animationTime, const NodeAnimation &nodeAnim);
	void CalcInterpolatedPosition(vec3 &out, float animationTime, const NodeAnimation &nodeAnim);
	void CalcInterpolatedRotation(quat &out, float animationTime, const NodeAnimation &nodeAnim);
	void CalcInterpolatedScaling(vec3 &out, float animationTime, const NodeAnimation &nodeAnim);

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
	unsigned int currentAnimation;

	std::vector<mat4> nodeTransformations;
	std::vector<unsigned int> nonSkinnedRenderablePartsNodeIndices;
	std::vector<unsigned int> skinnedRenderablePartsBoneIndices;
	unsigned int numNonSkinnedRenderableParts;
};

}