#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

namespace FireCube
{

// Forward declarations.
namespace Filesystem
{
   std::string FIRECUBE_API SearchForFileName(const std::string &filename);
}

/**
* A templated class for a resource pool.<br>
* Resources should define a bool Load(const string &) function to be usable.
*/
template<class T>
class ResourcePool
{
public:
	ResourcePool<T>()
	{

	}
	virtual ~ResourcePool<T>()
	{
		Logger::Write(Logger::LOG_INFO, "Destroying resource manager");
	}

	/**
	* Constructs the resource manager from another.
	* @param r The resource manager to copy.
	*/
	ResourcePool<T>(const ResourcePool<T> &r)
	{
		pool = r.pool;
	}

	/**
	* Creates and loads a resource from the specified file.
	* @param filename The file to load.
	*/
	std::shared_ptr<T> Create(const std::string &filename)
	{
		char pFullPathName[1024];
		string fullPathName;
		std::string loadfile = Filesystem::SearchForFileName(filename);
		if (loadfile.empty())
			return std::shared_ptr<T>();
		if (GetFullPathNameA(loadfile.c_str(),1024,pFullPathName,NULL) == 0)
			return std::shared_ptr<T>();
		fullPathName = pFullPathName;
		map<std::string, std::weak_ptr<T>>::iterator i = pool.find(fullPathName);
		if (i != pool.end())
			if (!i->second.expired())
				return i->second.lock();
		std::shared_ptr<T> ret(new T);
		if (ret->Load(loadfile))
		{						
			pool[fullPathName] = ret;
			return ret;
		}
		else
			return std::shared_ptr<T>();
	}

	/**
	* Adds an already existing resource.
	* @param filename The filename/name identifying the resource.
	* @param res The resource itself.
	*/
	void Add(const std::string &filename, std::shared_ptr<T> res)
	{
		if (pool.find(filename) != pool.end())
			return;

		pool[filename] = res;
	}

	/**
	* Returns a resource with a given filename, null if it does not exist.
	* @param filename The filename identifying the resource.
	*/
	std::shared_ptr<T> Get(const std::string &filename)
	{
		std::map<std::string, std::weak_ptr<T>>::iterator i = pool.find(filename);
		if (i != pool.end())
			if (!i->second.expired())
				return i->second.lock();

		return std::shared_ptr<T>();
	}
protected:

	/**
	* The list of resources contained in this resource pool.
	*/
	std::map<std::string, std::weak_ptr<T>> pool;
};
}
#endif