#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class Renderer;
class ResourceCache;
class InputManager;
class DebugRenderer;
class UI;

class FIRECUBE_API Engine
{
public:
	Engine();
	void SetRenderer(Renderer *renderer);
	Renderer *GetRenderer();
	void SetResourceCache(ResourceCache *resourceCache);
	ResourceCache *GetResourceCache();
	void SetInputManager(InputManager *inputManager);
	InputManager *GetInputManager();
	void SetDebugRenderer(DebugRenderer *debugRenderer);
	DebugRenderer *GetDebugRenderer();
	void SetUI(UI *ui);
	UI *GetUI();
private:
	Renderer *renderer;
	ResourceCache *resourceCache;
	InputManager *inputManager;
	DebugRenderer *debugRenderer;
	UI *ui;
};

}