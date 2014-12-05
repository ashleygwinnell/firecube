#include <sstream>

#include "Rendering/AnimatedModel.h"
#include "Scene/Node.h"
#include "Geometry/Geometry.h"
#include "Geometry/AnimatedMesh.h"
#include "Core/Events.h"
using namespace FireCube;

AnimatedModel::AnimatedModel(Engine *engine) : Renderable(engine), currentTime(0.0f)
{
	SubscribeToEvent(Events::Update, &AnimatedModel::Update);
}

AnimatedModel::AnimatedModel(Engine *engine, AnimatedMesh *mesh) : AnimatedModel(engine)
{
	CreateFromMesh(mesh);
}

void AnimatedModel::CreateFromMesh(AnimatedMesh *mesh)
{
	this->skeletonRoot = mesh->GetSkeletonRoot();
	this->meshBoneWeights = mesh->GetBoneWeights();
	this->meshBones = mesh->GetBones();
	this->animations = mesh->GetAnimations();

	skinMatrices.resize(mesh->GetGeometries().size());

	for (unsigned int i = 0; i < mesh->GetGeometries().size(); ++i)
	{
		Geometry *geometry = mesh->GetGeometries()[i];
		Material *material = mesh->GetMaterials()[i];
				
		geometries.push_back(geometry);
		materials.push_back(material);

		if (geometry->GetGeometryType() == GeometryType::SKINNED)
		{
			skinMatrices[i].resize(meshBones[i].size());
		}
	}
	
	boundingBoxes = mesh->GetBoundingBoxes();
	MarkedDirty();	
	nodeTransformations.resize(mesh->GetNumberOfTreeNodes());	

	// Create renderable parts based on the meshes in the skeleton
	std::vector<RenderablePart> skinned;
	CreateRenderableParts(skeletonRoot, renderableParts, skinned);
	numNonSkinnedRenderableParts = renderableParts.size();
	renderableParts.insert(renderableParts.end(), skinned.begin(), skinned.end());
	SetAnimation(0);
}

void AnimatedModel::Update(float time)
{
	currentTime += time;
	MarkedDirty();
}

void AnimatedModel::CreateRenderableParts(SkeletonNode &skeletonNode, std::vector<RenderablePart> &nonSkinned, std::vector<RenderablePart> &skinned)
{
	for (auto meshIndex : skeletonNode.meshes)
	{
		RenderablePart part;
		part.geometry = geometries[meshIndex];
		part.material = materials[meshIndex];
		if (part.geometry->GetGeometryType() == GeometryType::SKINNED)
		{
			part.skinMatricesCount = meshBones[meshIndex].size();
			part.skinMatrices = skinMatrices[meshIndex].data();
			skinned.push_back(part);
			skinnedRenderablePartsBoneIndices.push_back(meshIndex);			
		}
		else
		{
			
			nonSkinned.push_back(part);
			nonSkinnedRenderablePartsNodeIndices.push_back(skeletonNode.nodeIndex);
		}		
	}
	for (auto &c : skeletonNode.children)
	{
		CreateRenderableParts(c, nonSkinned, skinned);
	}
}

void AnimatedModel::SetAnimation(unsigned int index)
{
	currentAnimation = index;
	nodeAnimTransformations.resize(animations[currentAnimation].nodeAnimations.size());
	UpdateTreeNodeAnims(skeletonRoot);
}

void AnimatedModel::CalculateNodeAnimations(float animationTime)
{
	Animation &animation = animations[currentAnimation];
	unsigned int i = 0;
	for (auto &nodeAnim : animation.nodeAnimations)
	{
		// Interpolate scaling and generate scaling transformation matrix
		vec3 scaling;
		CalcInterpolatedScaling(scaling, animationTime, nodeAnim);
		mat4 scalingM = mat4::IDENTITY;
		scalingM.Scale(scaling.x, scaling.y, scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		quat rotationQ;
		CalcInterpolatedRotation(rotationQ, animationTime, nodeAnim);
		mat4 rotationM = rotationQ.GetMatrix().ToMat4();

		// Interpolate translation and generate translation transformation matrix
		vec3 translation;
		CalcInterpolatedPosition(translation, animationTime, nodeAnim);
		mat4 translationM = mat4::IDENTITY;
		translationM.Translate(translation);

		// Combine the above transformations
		nodeAnimTransformations[i++] = translationM * rotationM * scalingM;		
	}
}


void AnimatedModel::UpdateWorldBoundingBox()
{
	worldBoundingBox = BoundingBox();
	for (unsigned int i = 0; i < numNonSkinnedRenderableParts; ++i)
	{
		unsigned int geometryIndex = 0;
		for (unsigned int j = 0; j < geometries.size(); ++j)
		{
			if (renderableParts[i].geometry == geometries[j].Get())
			{
				geometryIndex = j;
				break;
			}
		}
		BoundingBox transformedBoundingBox = boundingBoxes[geometryIndex];
		transformedBoundingBox.Transform(nodeTransformations[nonSkinnedRenderablePartsNodeIndices[i]]);
		worldBoundingBox.Expand(transformedBoundingBox);

	}	
	worldBoundingBox.Transform(node->GetWorldTransformation());
}

void AnimatedModel::UpdateRenderableParts()
{
	unsigned int partIndex = 0;
	float ticksPerSecond = animations[0].ticksPerSecond != 0.0f ? animations[0].ticksPerSecond : 25.0f;
	float timeInTicks = currentTime * ticksPerSecond;
	float animationTime = fmod(timeInTicks, animations[0].duration);
	CalculateNodeAnimations(animationTime);
	BuildTreeTransformations(skeletonRoot, mat4::IDENTITY);
	UpdateSkinningMatrices();
	UpdateBoundingBox();
	for (unsigned int i = 0; i < numNonSkinnedRenderableParts; ++i)
	{
		renderableParts[i].transformation = node->GetWorldTransformation() * nodeTransformations[nonSkinnedRenderablePartsNodeIndices[i]];
	}

	for (unsigned int i = 0; i < renderableParts.size() - numNonSkinnedRenderableParts; ++i)
	{
		auto &renderablePart = renderableParts[i + numNonSkinnedRenderableParts];
		renderableParts[i].transformation = node->GetWorldTransformation();
	}	
}

void AnimatedModel::IntersectRay(RayQuery &rayQuery)
{
	float distance;
	if (rayQuery.ray.IntersectBoundingBox(GetWorldBoundingBox(), distance) && distance <= rayQuery.maxDistance)
	{
		mat4 worldTransform = node->GetWorldTransformation();
		worldTransform.Inverse();
		Ray localRay = rayQuery.ray.Transformed(worldTransform);
		vec3 normal, minNormal;
		float minDistance;
		bool found = false;
		for (auto geometry : geometries)
		{
			if (geometry->IntersectRay(localRay, distance, normal) && distance <= rayQuery.maxDistance)
			{
				if (!found || distance < minDistance)
				{
					found = true;
					minDistance = distance;
					minNormal = normal;
				}
			}
		}
		if (found)
		{
			RayQueryResult result;
			result.distance = minDistance;
			result.renderable = this;
			result.normal = minNormal;
			rayQuery.results.push_back(result);
		}
	}
}

void AnimatedModel::BuildTreeTransformations(SkeletonNode &node, mat4 parentTransform)
{
	mat4 worldTransformation;	
	mat4 nodeTransformation;
	
	if (node.hasNodeAnim)
	{
		nodeTransformation = nodeAnimTransformations[node.nodeAnimIndex];
	}
	else
	{
		nodeTransformation = node.transformation;
	}

	worldTransformation = parentTransform * nodeTransformation;

	nodeTransformations[node.nodeIndex] = worldTransformation;

	for (auto &c : node.children)
	{
		BuildTreeTransformations(c, worldTransformation);
	}
}

void AnimatedModel::UpdateTreeNodeAnims(SkeletonNode &node)
{
	node.hasNodeAnim = false;
	for (unsigned int nodeAnimIndex = 0; nodeAnimIndex < animations[currentAnimation].nodeAnimations.size(); ++nodeAnimIndex)
	{
		const auto &nodeAnim = animations[currentAnimation].nodeAnimations[nodeAnimIndex];
		if (nodeAnim.nodeName == node.name)
		{
			node.hasNodeAnim = true;
			node.nodeAnimIndex = nodeAnimIndex;
			break;
		}
	}

	for (auto &c : node.children)
	{
		UpdateTreeNodeAnims(c);
	}
}

void AnimatedModel::UpdateSkinningMatrices()
{
	for (unsigned int i = 0; i < meshBones.size(); ++i)
	{
		for (unsigned int j = 0; j < meshBones[i].size(); ++j)
		{
			skinMatrices[i][j] = nodeTransformations[meshBones[i][j].nodeIndex] * meshBones[i][j].offsetMarix;
		}
	}
}

void AnimatedModel::UpdateBoundingBox()
{

}

unsigned int AnimatedModel::FindPosition(float animationTime, const NodeAnimation &nodeAnim)
{
	if (animationTime < nodeAnim.positionAnimation[0].first)
		return 0;
	if (animationTime > nodeAnim.positionAnimation.back().first)
		return nodeAnim.positionAnimation.size() - 1;
	for (unsigned int i = 0; i < nodeAnim.positionAnimation.size() - 1; i++)
	{
		if (animationTime < nodeAnim.positionAnimation[i + 1].first)
		{
			return i;
		}
	}

	return 0;
}


unsigned int AnimatedModel::FindRotation(float animationTime, const NodeAnimation &nodeAnim)
{
	if (animationTime < nodeAnim.rotationAnimation[0].first)
		return 0;
	if (animationTime > nodeAnim.rotationAnimation.back().first)
		return nodeAnim.rotationAnimation.size() - 1;

	for (unsigned int i = 0; i < nodeAnim.rotationAnimation.size() - 1; i++)
	{
		if (animationTime < nodeAnim.rotationAnimation[i + 1].first)
		{
			return i;
		}
	}

	return 0;
}


unsigned int AnimatedModel::FindScaling(float animationTime, const NodeAnimation &nodeAnim)
{
	if (animationTime < nodeAnim.scaleAnimation[0].first)
		return 0;
	if (animationTime > nodeAnim.scaleAnimation.back().first)
		return nodeAnim.scaleAnimation.size() - 1;

	for (unsigned int i = 0; i < nodeAnim.scaleAnimation.size() - 1; i++)
	{
		if (animationTime < nodeAnim.scaleAnimation[i + 1].first)
		{
			return i;
		}
	}

	return 0;
}


void AnimatedModel::CalcInterpolatedPosition(vec3 &out, float animationTime, const NodeAnimation &nodeAnim)
{
	if (nodeAnim.positionAnimation.size() == 1)
	{
		out = nodeAnim.positionAnimation[0].second;
		return;
	}

	unsigned int positionIndex = FindPosition(animationTime, nodeAnim);
	unsigned int nextPositionIndex = (positionIndex + 1) % nodeAnim.positionAnimation.size();

	float deltaTime = (float)(nodeAnim.positionAnimation[nextPositionIndex].first - nodeAnim.positionAnimation[positionIndex].first);
	if (deltaTime < 0.0f)
		deltaTime += animations[0].duration;
	float factor = (animationTime - (float)nodeAnim.positionAnimation[positionIndex].first) / deltaTime;

	const vec3 &start = nodeAnim.positionAnimation[positionIndex].second;
	const vec3 &end = nodeAnim.positionAnimation[nextPositionIndex].second;
	vec3 delta = end - start;
	out = start + factor * delta;
}


void AnimatedModel::CalcInterpolatedRotation(quat &out, float animationTime, const NodeAnimation &nodeAnim)
{
	// we need at least two values to interpolate...
	if (nodeAnim.rotationAnimation.size() == 1)
	{
		out = nodeAnim.rotationAnimation[0].second;
		return;
	}

	unsigned int rotationIndex = FindRotation(animationTime, nodeAnim);
	unsigned int nextRotationIndex = (rotationIndex + 1) % nodeAnim.rotationAnimation.size();

	float deltaTime = (float)(nodeAnim.rotationAnimation[nextRotationIndex].first - nodeAnim.rotationAnimation[rotationIndex].first);
	if (deltaTime < 0.0f)
		deltaTime += animations[0].duration;
	float factor = (animationTime - (float)nodeAnim.rotationAnimation[rotationIndex].first) / deltaTime;

	const quat &startRotationQ = nodeAnim.rotationAnimation[rotationIndex].second;
	const quat &endRotationQ = nodeAnim.rotationAnimation[nextRotationIndex].second;
	quat::Interpolate(out, startRotationQ, endRotationQ, factor);
	out.Normalize();
}


void AnimatedModel::CalcInterpolatedScaling(vec3 &out, float animationTime, const NodeAnimation &nodeAnim)
{
	if (nodeAnim.scaleAnimation.size() == 1)
	{
		out = nodeAnim.scaleAnimation[0].second;
		return;
	}

	unsigned int scalingIndex = FindScaling(animationTime, nodeAnim);
	unsigned int nextScalingIndex = (scalingIndex + 1) % nodeAnim.scaleAnimation.size();

	float deltaTime = (float)(nodeAnim.scaleAnimation[nextScalingIndex].first - nodeAnim.scaleAnimation[scalingIndex].first);
	if (deltaTime < 0.0f)
		deltaTime += animations[0].duration;
	float factor = (animationTime - (float)nodeAnim.scaleAnimation[scalingIndex].first) / deltaTime;

	const vec3 &start = nodeAnim.scaleAnimation[scalingIndex].second;
	const vec3 &end = nodeAnim.scaleAnimation[nextScalingIndex].second;
	vec3 delta = end - start;
	out = start + factor * delta;
}