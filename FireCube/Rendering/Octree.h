#pragma once

#include "Core/Object.h"
#include "Utils/utils.h"
#include "Scene/Renderable.h"

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
	OBJECT(Octree)
public:
	Octree(Engine *engine);
};

}