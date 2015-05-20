#include "SceneView.h"
#include "Core/Engine.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

SceneView::SceneView(Engine *engine, Scene *scene, Camera *camera, SharedPtr<RenderSurface> renderSurface, RenderPath *renderPath) : Object(engine), scene(scene), camera(camera),
																																	 renderSurface(renderSurface), renderPath(renderPath)
{
	if (this->renderPath == nullptr)
	{
		this->renderPath = engine->GetRenderer()->GetDefaultRenderPath();
	}
}

Scene *SceneView::GetScene()
{
	return scene;
}

Camera *SceneView::GetCamera()
{
	return camera;
}

RenderSurface *SceneView::GetRenderSurface()
{
	return renderSurface;
}

RenderPath *SceneView::GetRenderPath()
{
	return renderPath;
}