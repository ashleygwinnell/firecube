#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class Renderer;
class ResourcePool;
class InputManager;
class DebugRenderer;

class FIRECUBE_API Engine
{
public:
	Engine();
	void SetRenderer(Renderer *renderer);
	Renderer *GetRenderer();
	void SetResourcePool(ResourcePool *resourcePool);
	ResourcePool *GetResourcePool();
	void SetInputManager(InputManager *inputManager);
	InputManager *GetInputManager();
	void SetDebugRenderer(DebugRenderer *debugRenderer);
	DebugRenderer *GetDebugRenderer();
private:
	Renderer *renderer;
	ResourcePool *resourcePool;
	InputManager *inputManager;
	DebugRenderer *debugRenderer;
};

}