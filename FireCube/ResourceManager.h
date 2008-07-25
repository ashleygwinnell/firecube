#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
template<class T>
class ResourceManager
{
public:	
	ResourceManager<T>()
	{

	}
	virtual ~ResourceManager<T>()
	{

	}
	ResourceManager<T>(const ResourceManager<T> &r)
	{
		resources=r.resources;
	}
	boost::shared_ptr<T> Create(const string &filename)
	{
		map<string,boost::weak_ptr<T>>::iterator i=resources.find(filename);
		if (i!=resources.end())
			if (!i->second.expired())
				return i->second.lock();
		boost::shared_ptr<T> ret(new T);
		if (ret->Load(filename))
		{		
			resources[filename]=ret;
			return ret;
		}
		else
			return boost::shared_ptr<T>();
	}
	void Add(const string &filename,boost::shared_ptr<T> res)
	{
		if (resources.find(filename)!=resources.end())
			return;

		resources[filename]=res;
	}
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
#endif