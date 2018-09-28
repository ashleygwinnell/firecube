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
class Camera;

/**
* This class is reponsible for rendering a single frame given a scene, a camera using a render path
*/
class FIRECUBE_API Frame : public Object
{
	FIRECUBE_OBJECT(Frame)
public:
	Frame(Engine *engine, Scene *scene, Camera *camera, RenderSurface *renderSurface, RenderPath *renderPath);
	
	/**
	* Renders the scene
	* @param renderer The renderer to use
	*/
	void Render(Renderer *renderer);
	
	/**
	* Renders debug geometry of the scene
	* @param debugRenderer The debug renderer
	*/
	void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	/**
	* Sets the render target to render into instead of the viewport for render path commands which output to the viewport
	* @param renderSurface A render surface to render into or nullptr to render into the default framebuffer
	*/
	void SetRenderTarget(SharedPtr<RenderSurface> renderSurface);
private:
	void UpdateBaseQueue();
	void UpdateLightQueues();
	void SetRenderTargets(Renderer *renderer, const RenderPathCommand &command);
	void SetTextures(Renderer *renderer, const RenderPathCommand &command);
	void RenderDebugGeometry(DebugRenderer *debugRenderer, Node *node);
	void RenderShadowMap(Renderer *renderer, Light *light, RenderQueue &queue);
	void RenderUI(Renderer *renderer);

	Scene *scene;
	Camera *camera;
	RenderSurface *renderSurface;
	RenderPath *renderPath;
	std::vector<std::pair<Light *, std::map<StringHash, RenderQueue>>> lightQueues;
	std::map<StringHash, RenderQueue> baseQueues;
};

}