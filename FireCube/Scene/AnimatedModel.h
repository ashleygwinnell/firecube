#pragma once

#include "Scene/Renderable.h"
#include "Geometry/Mesh.h"

namespace FireCube
{

class Mesh;

/**
* This class represnets an animated model component
*/
class AnimatedModel : public Renderable
{
	FIRECUBE_OBJECT(AnimatedModel)
public:
	AnimatedModel(Engine *engine);
	AnimatedModel(Engine *engine, Mesh *mesh);

	/**
	* Creates the model from a mesh resource
	* @param mesh The mesh to use
	*/
	void CreateFromMesh(Mesh *mesh);

	/**
	* Intersects the model with a ray
	* @param rayQuery The ray query to use
	*/
	virtual void IntersectRay(RayQuery &rayQuery);			
	
	/**
	* Sets the animation index to play
	* @param index The animation index
	*/		
	void SetAnimation(unsigned int index);
	
	/**
	* Clones this component
	*/
	virtual Component *Clone() const;
private:

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

	virtual void UpdateRenderableParts();
	
	/**
	* Updates the animation
	* @param dt The time passed
	*/
	void Update(float dt);

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