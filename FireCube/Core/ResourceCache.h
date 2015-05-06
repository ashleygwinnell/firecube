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
	OBJECT(ResourceCache)
public:
	ResourceCache(Engine *engine);

	~ResourceCache();

	template <class T> T *GetResource(const std::string &filename)
	{	
		std::string name = Filesystem::SearchForFileName(filename);
		if (name.empty())
			return nullptr;

		auto i = resources.find(name);
		if (i != resources.end())
			return (T *) i->second.Get();

		SharedPtr<T> resource = SharedPtr<T>(new T(engine));
		if (!resource->Load(name))
			return nullptr;

		resource->SetFileName(filename);
		SharedPtr<Resource> resourcePtr;
		resourcePtr.StaticCast(resource);
		resources[name] = resourcePtr;
		return resource;
	}

private:
	std::map<std::string, SharedPtr<Resource>> resources;
};


}
