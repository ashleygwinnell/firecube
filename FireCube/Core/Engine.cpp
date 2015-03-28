#include "Engine.h"
using namespace FireCube;

Engine::Engine() : renderer(nullptr), resourceCache(nullptr)
{

}

void Engine::SetRenderer(Renderer *renderer)
{
	this->renderer = renderer;
}
Renderer *Engine::GetRenderer()
{
	return renderer;
}

void Engine::SetResourceCache(ResourceCache *resourceCache)
{
	this->resourceCache = resourceCache;
}

ResourceCache *Engine::GetResourceCache()
{
	return resourceCache;
}

void Engine::SetInputManager(InputManager *inputManager)
{
	this->inputManager = inputManager;
}

InputManager *Engine::GetInputManager()
{
	return inputManager;
}

void Engine::SetDebugRenderer(DebugRenderer *debugRenderer)
{
	this->debugRenderer = debugRenderer;
}

DebugRenderer *Engine::GetDebugRenderer()
{
	return debugRenderer;
}

void Engine::SetUI(UI *ui)
{
	this->ui = ui;
}

UI *Engine::GetUI()
{
	return ui;
}

void Engine::SetLuaState(LuaState *luaState)
{
	this->luaState = luaState;
}

LuaState *Engine::GetLuaState()
{
	return luaState;
}