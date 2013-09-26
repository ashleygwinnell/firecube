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