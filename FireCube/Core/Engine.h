#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class Renderer;
class ResourcePool;

class FIRECUBE_API Engine
{
public:
	Engine();
	void SetRenderer(Renderer *renderer);
	Renderer *GetRenderer();
	void SetResourcePool(ResourcePool *resourcePool);
	ResourcePool *GetResourcePool();
private:
	Renderer *renderer;
	ResourcePool *resourcePool;

};

}