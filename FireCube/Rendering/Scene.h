#pragma once

#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Geometry/CollisionQuery.h"

namespace FireCube
{

class Renderable;
class DebugRenderer;

class Scene
{
public:
	Scene();
	~Scene();
	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);
	void AddLight(Light *light);
	void Render(Renderer *renderer);
	void RenderDebugGeometry(DebugRenderer *debugRenderer);
	void SetRootNodeAndCamera(NodePtr rootNode, CameraPtr camera);
	void SetAmbientColor(vec3 color);
	void IntersectRay(RayQuery &rayQuery);
private:	
	void UpdateRenderables();
	void UpdateBaseQueue();
	void UpdateLightQueues();

	NodePtr rootNode;
	CameraPtr camera;
	std::vector<Renderable *> renderables;
	std::vector<Light *> lights;
	std::vector<LightQueue> lightQueues;
	std::vector<std::string> scenePasses;
	RenderQueue baseQueue;
	vec3 ambientColor;	
};

}