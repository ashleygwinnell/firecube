#pragma once

#include <memory>
#include <map>
#include "Core/Object.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"

namespace FireCube
{

class Resource;
typedef std::shared_ptr<Resource> ResourcePtr;

class ResourcePool : public Object
{
public:
	ResourcePool(Engine *engine);

	~ResourcePool()
	{
		LOGINFO("Destroying resource pool");
	}


	template <class T> std::shared_ptr<T> GetResource(const std::string &filename)
	{	
		std::string name = Filesystem::SearchForFileName(filename);
		if (name.empty())
			return std::shared_ptr<T>();

		std::map<std::string, ResourcePtr>::iterator i = resources.find(name);
		if (i != resources.end())
			return std::static_pointer_cast<T>(i->second);

		std::shared_ptr<T> resource(new T(engine));
		if (!resource->Load(name))
			return std::shared_ptr<T>();
		resources[name] = resource;
		return resource;
	}

private:
	std::map<std::string, ResourcePtr> resources;
};


}
