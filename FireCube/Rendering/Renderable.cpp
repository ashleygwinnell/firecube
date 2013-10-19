#include "Renderable.h"
#include "Scene/Node.h"
#include "Rendering/Scene.h"
#include "Rendering/DebugRenderer.h"

using namespace FireCube;

Renderable::Renderable(Engine *engine) : Component(engine), worldBoundingBoxChanged(false), queryIntersection(true)
{
}

Renderable::~Renderable()
{
}

const std::vector<RenderablePart> &Renderable::GetRenderableParts() const
{
	return renderableParts;
}

void Renderable::SetScene(Scene *scene)
{
	this->scene = scene;
}

void Renderable::SetQueryIntersection(bool queryIntersection)
{
	this->queryIntersection = queryIntersection;
}

bool Renderable::GetQueryIntersection() const
{
	return queryIntersection;
}

void Renderable::NodeChanged()
{
	if (node && node->GetScene())
	{
		node->GetScene()->AddRenderable(this);	
		scene = node->GetScene();
	}
	else if (!node && scene)
	{
		scene->RemoveRenderable(this);
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