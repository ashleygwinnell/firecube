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
public:
	ResourceCache(Engine *engine);

	~ResourceCache();

	template <class T> T *GetResource(const std::string &filename)
	{	
		std::string name = Filesystem::SearchForFileName(filename);
		if (name.empty())
			return nullptr;

		std::map<std::string, Resource*>::iterator i = resources.find(name);
		if (i != resources.end())
			return (T *) i->second;

		T *resource = new T(engine);
		if (!resource->Load(name))
			return nullptr;
		resources[name] = resource;
		return resource;
	}

private:
	std::map<std::string, Resource *> resources;
};


}
