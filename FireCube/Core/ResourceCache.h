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
	* Loads a resource from a path or fetches it from cache if the resource was previously loaded
	* @param filename The path to the resource (this path is also used as the key to look up the resource in the cache)
	* @returns A pointer to the loaded resource or null if the resource couldn't be loaded
	*/
	template <class T> T *GetResource(const std::string &filename)
	{
		StringHash typeHash = T::GetTypeStatic();
		auto group = resources.find(typeHash);
		if (group != resources.end())
		{
			auto i = group->second.find(filename);
			if (i != group->second.end())
			{
				return (T *)i->second.Get();
			}
		}

		SharedPtr<T> resource = SharedPtr<T>(new T(engine));
		if (!resource->Load(filename))
			return nullptr;

		resource->SetFileName(filename);
		SharedPtr<Resource> resourcePtr;
		resourcePtr.StaticCast(resource);
		resources[typeHash][filename] = resourcePtr;
		return resource;
	}

	template <class T> T *FindResource(const std::string &filename) const
	{
		StringHash typeHash = T::GetTypeStatic();
		auto group = resources.find(typeHash);
		if (group != resources.end())
		{
			auto i = group->second.find(filename);
			if (i != group->second.end())
			{
				return (T *)i->second.Get();
			}
		}
		
		return nullptr;
	}

	bool ReloadResource(Resource *resource);

private:
	std::map<StringHash, std::map<std::string, SharedPtr<Resource>>> resources;
};


}
