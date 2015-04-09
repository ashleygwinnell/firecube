#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class Renderer;
class ResourceCache;
class InputManager;
class DebugRenderer;
class UI;
class LuaState;

class FIRECUBE_API Engine
{
public:
	Engine();
	void SetRenderer(Renderer *renderer);
	Renderer *GetRenderer() const;
	void SetResourceCache(ResourceCache *resourceCache);
	ResourceCache *GetResourceCache() const;
	void SetInputManager(InputManager *inputManager);
	InputManager *GetInputManager() const;
	void SetDebugRenderer(DebugRenderer *debugRenderer);
	DebugRenderer *GetDebugRenderer() const;
	void SetUI(UI *ui);
	UI *GetUI() const;
	void SetLuaState(LuaState *luaState);
	LuaState *GetLuaState() const;
private:
	Renderer *renderer;
	ResourceCache *resourceCache;
	InputManager *inputManager;
	DebugRenderer *debugRenderer;
	UI *ui;
	LuaState *luaState;
};

}