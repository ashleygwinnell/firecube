#include "Core/ResourceCache.h"
#include "Utils/Filesystem.h"
#include "Core/Resource.h"
#include "Core/Events.h"
using namespace FireCube;


ResourceCache::ResourceCache(Engine *engine) : Object(engine)
{

}

ResourceCache::~ResourceCache()
{
	
}

bool ResourceCache::ReloadResource(Resource *resource)
{	
	if (resource->Load(resource->GetFileName()))
	{
		Events::ResourceReloaded(resource, resource);
		return true;
	}
	else
	{
		return false;
	}
}
