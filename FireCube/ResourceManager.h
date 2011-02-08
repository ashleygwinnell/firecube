#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

namespace FireCube
{
/**
* A templated class for a resource manager.<br>
* Resources should define a bool Load(const string &) function to be usable.
*/
template<class T,class TResource>
class ResourceManager
{
public:	
	ResourceManager<T,TResource>()
	{

	}
	virtual ~ResourceManager<T,TResource>()
	{
		Logger::Write(Logger::LOG_INFO, "Destroying resource manager");
	}
	
	/**
	* Constructs the resource manager from another.
	* @param r The resource manager to copy.
	*/
	ResourceManager<T,TResource>(const ResourceManager<T,TResource> &r)
	{
		resources=r.resources;
	}
	
	/**
	* Creates and loads a resource from the specified file.
	* @param filename The file to load.
	*/
	T Create(const std::string &filename)
	{
		map<std::string,boost::weak_ptr<TResource>>::iterator i=resources.find(filename);
		if (i!=resources.end())
			if (!i->second.expired())
				return T(i->second.lock());
		T ret;
		std::string loadfile=Application::SearchForFileName(filename);
		if (loadfile.empty())
			return T();
		if (ret.Load(loadfile))
		{		
			resources[filename]=ret.resource;
			return ret;
		}
		else
			return T();
	}
	
	/**
	* Adds a new resource without loading it.
	* @param name The name identifying the resource.
	* @return The newly created resource.
	*/
	T Add(const std::string &name)
	{
		std::map<std::string,boost::weak_ptr<TResource>>::iterator i=resources.find(name);
		if (i!=resources.end())
			if (!i->second.expired())
				return T(i->second.lock());
		T ret;
		resources[name]=ret.resource;
		return ret;		
	}
	
	/**
	* Adds an already existing resource.
	* @param filename The filename/name identifying the resource.
	* @param res The resource itself.
	*/
	void Add(const std::string &filename,T res)
	{
		if (resources.find(filename)!=resources.end())
			return;

		resources[filename]=res.resource;
	}
	
	/**
	* Returns a resource with a given filename, null if it does not exist.
	* @param filename The filename identifying the resource.
	*/
	T Get(const std::string &filename)
	{
		std::map<std::string,boost::weak_ptr<TResource>>::iterator i=resources.find(filename);
		if (i!=resources.end())
			if (!i->second.expired())
				return T(i->second.lock());
		
		return T();
	}
protected:
	
	/**
	* The list of resources contained in this resource manager.
	*/
	std::map<std::string,boost::weak_ptr<TResource>> resources;
};
}
#endif