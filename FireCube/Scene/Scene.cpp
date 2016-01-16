#include <algorithm>

#include "Scene/Renderable.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Technique.h"
#include "Rendering/ShaderTemplate.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/RenderPath.h"
#include "Rendering/RenderSurface.h"
#include "Geometry/Geometry.h"
#include "Scene/Camera.h"
#include "Scene.h"
#include "Utils/Logger.h"
#include "Core/Engine.h"

using namespace FireCube;

Scene::Scene(Engine *engine) : Object(engine), ambientColor(0.1f), fogEnabled(false), rootNode(engine), fogColor(1.0f), octree(engine, vec3(2000), 8)
{
	rootNode.SetScene(this);
}

Scene::~Scene()
{
	for (auto renderable : renderables)
	{
		renderable->SetScene(nullptr);
	}

	for (auto light : lights)
	{
		light->SetScene(nullptr);
	}
}

void Scene::AddRenderable(Renderable *renderable)
{
	renderables.push_back(renderable);
	octree.Insert(renderable);
}

void Scene::RemoveRenderable(Renderable *renderable)
{
	for (auto i = renderables.begin(); i != renderables.end(); ++i)
	{
		if (*i == renderable)
		{
			renderables.erase(i);
			if (renderable->GetOctreeNode())
			{
				octree.Remove(renderable);
			}
			break;
		}
	}
}

void Scene::UpdateRenderables()
{
	for (auto renderable : renderables)
	{
		renderable->UpdateRenderableParts();
	}

	octree.Update();
}

void Scene::AddLight(Light *light)
{
	lights.push_back(light);
}

void Scene::RemoveLight(Light *light)
{
	for (auto i = lights.begin(); i != lights.end(); ++i)
	{
		if (*i == light)
		{
			lights.erase(i);
			break;
		}
	}
}

void Scene::SetAmbientColor(vec3 color)
{
	ambientColor = color;
}

void Scene::IntersectRay(RayQuery &rayQuery, unsigned int collisionQueryMask)
{
	std::vector<Renderable *> closeRenderables;
	octree.GetObjects(rayQuery.ray, rayQuery.maxDistance, closeRenderables);
	for (auto renderable : closeRenderables)
	{
		if (renderable->GetCollisionQueryMask() & collisionQueryMask)
			renderable->IntersectRay(rayQuery);
	}

	std::sort(rayQuery.results.begin(), rayQuery.results.end(),
		[](const RayQueryResult &a, const RayQueryResult &b)
	{ 
		return a.distance < b.distance; 
	});
}

void Scene::SetFogEnabled(bool fogEnabled)
{
	this->fogEnabled = fogEnabled;
}

void Scene::SetFogParameters(vec3 fogParameters)
{
	this->fogParameters = fogParameters;
}

void Scene::SetFogColor(vec3 fogColor)
{
	this->fogColor = fogColor;
}

vec3 Scene::GetFogColor() const
{
	return fogColor;
}

Node *Scene::GetRootNode()
{
	return &rootNode;
}

std::vector<Renderable *> &Scene::GetRenderables()
{
	return renderables;
}

std::vector<Light *> &Scene::GetLights()
{
	return lights;
}

bool Scene::GetFogEnabled() const
{
	return fogEnabled;
}

vec3 Scene::GetFogParameters() const
{
	return fogParameters;
}

vec3 Scene::GetAmbientColor() const
{
	return ambientColor;
}

Octree<Renderable> &Scene::GetOctree()
{
	return octree;
}
