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

/**
* This class stores a scene to be rendered along with the camera to use and other properties needed for rendering
*/
class FIRECUBE_API SceneView : public Object
{
	FIRECUBE_OBJECT(SceneView);
public:
	SceneView(Engine *engine, Scene *scene, Camera *camera, SharedPtr<RenderSurface> renderSurface = nullptr, RenderPath *renderPath = nullptr);

	/**
	* @returns The scene of this scene view
	*/
	Scene *GetScene();

	/**
	* @returns The camera of this scene view
	*/
	Camera *GetCamera();

	/**
	* @returns The rendr target of this scene view
	*/
	RenderSurface *GetRenderSurface();

	/**
	* @returns The render path of this scene view
	*/
	RenderPath *GetRenderPath();
private:
	Scene *scene;
	Camera *camera;
	SharedPtr<RenderSurface> renderSurface;
	RenderPath *renderPath;
};

}