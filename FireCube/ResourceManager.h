#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

namespace FireCube
{
/**
* A templated class for a resource manager.<br>
* Resources should define a bool Load(const string &) function to be usable.
*/
template<class T>
class ResourceManager
{
public:	
	ResourceManager<T>()
	{

	}
	virtual ~ResourceManager<T>()
	{
		Logger::Write("Destroying resource manager.\n");
	}
	ResourceManager<T>(const ResourceManager<T> &r)
	{
		resources=r.resources;
	}
	/**
	* Creates and loads a resource from the specified file.
	* @param filename The file to load.
	*/
	boost::shared_ptr<T> Create(const string &filename)
	{
		map<string,boost::weak_ptr<T>>::iterator i=resources.find(filename);
		if (i!=resources.end())
			if (!i->second.expired())
				return i->second.lock();
		boost::shared_ptr<T> ret(new T);
		string loadfile=filename;
		string fname=GetFileName(filename);
		const vector<string> &searchPaths=Application::GetSearchPaths();
		if (!FileExists(loadfile))
		{
			for (unsigned int i=0;i<searchPaths.size();i++)
				if (FileExists(searchPaths[i] + "\\" + fname))
				{
					loadfile=searchPaths[i] + "\\" + fname;
					break;
				}
		}
		if (ret->Load(loadfile))
		{		
			resources[filename]=ret;
			return ret;
		}
		else
			return boost::shared_ptr<T>();
	}
	/**
	* Adds a new resource without loading it.
	* @param name The name identifying the resource.
	* @return The newly created resource.
	*/
	boost::shared_ptr<T> Add(const string &name)
	{
		map<string,boost::weak_ptr<T>>::iterator i=resources.find(name);
		if (i!=resources.end())
			if (!i->second.expired())
				return i->second.lock();
		boost::shared_ptr<T> ret(new T);
		resources[name]=ret;
		return ret;		
	}
	/**
	* Adds an already existing resource.
	* @param filename The filename/name identifying the resource.
	* @param res The resource itself.
	*/
	void Add(const string &filename,boost::shared_ptr<T> res)
	{
		if (resources.find(filename)!=resources.end())
			return;

		resources[filename]=res;
	}
	/**
	* Returns a resource with a given filename, null if it does not exist.
	* @param filename The filename identifying the resource.
	*/
	boost::shared_ptr<T> Get(const string &filename)
	{
		map<string,boost::weak_ptr<T>>::iterator i=resources.find(filename);
		if (i!=resources.end())
			if (!i->second.expired())
				return i->second.lock();
		
		return boost::shared_ptr<T>();		
	}
private:
	map<string,boost::weak_ptr<T>> resources;
};
}
#endif