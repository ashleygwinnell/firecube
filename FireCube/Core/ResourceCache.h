#pragma once

#include <memory>
#include <map>
#include "Core/Object.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Core/Resource.h"

namespace FireCube
{

class ResourceCache : public Object
{
	FIRECUBE_OBJECT(ResourceCache)
public:
	ResourceCache(Engine *engine);

	~ResourceCache();

	/**
	* Loads a resource from a path or fetches it from cache if the resource was previosly loaded
	* @param filename The path to the resource (this path is also used as the kay to look up the resource in the cache)
	* @returns A pointer to the loaded resource or null if the resource coundn't be loaded
	*/
	template <class T> T *GetResource(const std::string &filename)
	{	
		auto i = resources.find(filename);
		if (i != resources.end())
			return (T *) i->second.Get();

		SharedPtr<T> resource = SharedPtr<T>(new T(engine));
		if (!resource->Load(filename))
			return nullptr;

		resource->SetFileName(filename);
		SharedPtr<Resource> resourcePtr;
		resourcePtr.StaticCast(resource);
		resources[filename] = resourcePtr;
		return resource;
	}

private:
	std::map<std::string, SharedPtr<Resource>> resources;
};


}
