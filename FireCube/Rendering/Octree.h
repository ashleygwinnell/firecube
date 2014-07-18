#pragma once

#include "Core/Object.h"
#include "Utils/utils.h"
#include "Rendering/Renderable.h"

namespace FireCube
{

class FIRECUBE_API OctreeNode
{
public:
private:
	std::vector<Renderable *> renderables;
};

class FIRECUBE_API Octree : public Object
{
public:
	Octree(Engine *engine);
};

}