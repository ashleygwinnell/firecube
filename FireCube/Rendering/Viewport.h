#pragma once

#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"

namespace FireCube
{

class Renderable;

class Viewport
{
public:
	Viewport();
	void AddRenderable(Renderable *renderable);
	void AddLight(Light *light);
	void Render(Renderer *renderer);
	void SetRootNodeAndCamera(NodePtr rootNode, CameraPtr camera);
	void SetAmbientColor(vec3 color);
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