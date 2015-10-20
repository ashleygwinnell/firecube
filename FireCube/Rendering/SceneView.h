#pragma once

#include "Utils/utils.h"
#include "Core/Object.h"
#include "Core/Memory.h"
#include "Rendering/RenderSurface.h"

namespace FireCube
{

class Scene;
class Camera;
class RenderPath;

class FIRECUBE_API SceneView : public Object
{
	FIRECUBE_OBJECT(SceneView);
public:
	SceneView(Engine *engine, Scene *scene, Camera *camera, SharedPtr<RenderSurface> renderSurface = nullptr, RenderPath *renderPath = nullptr);
	Scene *GetScene();
	Camera *GetCamera();
	RenderSurface *GetRenderSurface();
	RenderPath *GetRenderPath();
private:
	Scene *scene;
	Camera *camera;
	SharedPtr<RenderSurface> renderSurface;
	RenderPath *renderPath;
};

}