#include "Renderable.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Rendering/DebugRenderer.h"

using namespace FireCube;

Renderable::Renderable(Engine *engine) : Component(engine), worldBoundingBoxChanged(true), collisionQueryMask(0xFFFFFFFF), castShadow(true), receiveShadow(true), lightMask(0xFFFFFFFF)
{
}

Renderable::Renderable(const Renderable &other) : Component(other), worldBoundingBoxChanged(true), collisionQueryMask(other.collisionQueryMask), lightMask(other.lightMask), castShadow(other.castShadow),
												  receiveShadow(other.receiveShadow)
{

}

Renderable::~Renderable()
{
}

const std::vector<RenderablePart> &Renderable::GetRenderableParts() const
{
	return renderableParts;
}

void Renderable::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
}

unsigned int Renderable::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void Renderable::NodeChanged()
{
	
}

void Renderable::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		oldScene->RemoveRenderable(this);
	}

	if (scene)
	{
		scene->AddRenderable(this);
	}	
}

void Renderable::EnabledChanged()
{
	if (IsEnabled() && scene == nullptr)
	{
		node->GetScene()->AddRenderable(this);
		scene = node->GetScene();
	}
	else if (IsEnabled() == false && scene)
	{
		scene->RemoveRenderable(this);		
		scene = nullptr;
	}
}

void Renderable::UpdateRenderableParts()
{
	for (auto &i : renderableParts)
	{
		i.transformation = node->GetWorldTransformation();
	}
}

BoundingBox Renderable::GetWorldBoundingBox()
{
	if (worldBoundingBoxChanged)
	{
		worldBoundingBoxChanged = false;
		UpdateWorldBoundingBox();
	}
	return worldBoundingBox;
}

void Renderable::MarkedDirty()
{	
	MarkForOctreeReinsertion();
	worldBoundingBoxChanged = true;
}

void Renderable::IntersectRay(RayQuery &rayQuery)
{
	float distance;
	if (rayQuery.ray.IntersectBoundingBox(GetWorldBoundingBox(), distance) && distance <= rayQuery.maxDistance)
	{		
		RayQueryResult result;
		result.distance = distance;
		result.renderable = this;
		rayQuery.results.push_back(result);
	}
}

void Renderable::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	debugRenderer->AddBoundingBox(GetWorldBoundingBox(), vec3(0, 1, 0));
}

void Renderable::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
}

bool Renderable::GetCastShadow() const
{
	return castShadow;
}

void Renderable::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
}

unsigned int Renderable::GetLightMask() const
{
	return lightMask;
}

void Renderable::SetReceiveShadow(bool receiveShadow)
{
	this->receiveShadow = receiveShadow;
}

bool Renderable::GetReceiveShadow() const
{
	return receiveShadow;
}