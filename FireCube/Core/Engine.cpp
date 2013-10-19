#include "Engine.h"
using namespace FireCube;

Engine::Engine() : renderer(nullptr), resourcePool(nullptr)
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

void Engine::SetResourcePool(ResourcePool *resourcePool)
{
	this->resourcePool = resourcePool;
}

ResourcePool *Engine::GetResourcePool()
{
	return resourcePool;
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