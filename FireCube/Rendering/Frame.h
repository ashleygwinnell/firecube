#pragma once

#include "Core/Object.h"
#include "Utils/utils.h"
#include "Rendering/RenderPath.h"
#include "Rendering/RenderQueue.h"

namespace FireCube
{

class Scene;
class Renderer;
class RenderSurface;

class FIRECUBE_API Frame : public Object
{
	OBJECT(Frame)
public:
	Frame(Engine *engine, Scene *scene);
	void Render(Renderer *renderer);
	void RenderDebugGeometry(DebugRenderer *debugRenderer);
	void SetRenderTarget(SharedPtr<RenderSurface> renderSurface);
private:
	void UpdateBaseQueue();
	void UpdateLightQueues();
	void SetRenderTargets(Renderer *renderer, const RenderPathCommand &command);
	void SetTextures(Renderer *renderer, const RenderPathCommand &command);
	void RenderDebugGeometry(DebugRenderer *debugRenderer, Node *node);
	void RenderShadowMap(Renderer *renderer, Light *light, RenderQueue &queue);	

	Scene *scene;
	SharedPtr<RenderSurface> renderSurface;
	std::vector<std::pair<Light *, std::map<StringHash, RenderQueue>>> lightQueues;
	std::map<StringHash, RenderQueue> baseQueues;
};

}