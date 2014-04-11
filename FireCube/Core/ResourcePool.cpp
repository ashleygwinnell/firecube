#include "Core/ResourcePool.h"
#include "Utils/Filesystem.h"
#include "Core/Resource.h"
using namespace FireCube;


ResourcePool::ResourcePool(Engine *engine) : Object(engine)
{

}

ResourcePool::~ResourcePool()
{
	LOGINFO("Destroying resource pool");
	for (auto i : resources)
		delete i.second;
}