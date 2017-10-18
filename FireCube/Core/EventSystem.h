#pragma once

#include <vector>
#include <algorithm>

namespace FireCube
{

class Object;
template<class... Args> class Event;

class BaseCallback
{
public:
	virtual ~BaseCallback(){ }
	virtual void *GetEvent() { return nullptr; }
};

template<class... Args> class AbstractCallback : public BaseCallback
{
protected:
	AbstractCallback() : v(nullptr) {}
	virtual ~AbstractCallback() {};

	friend class Event < Args... > ;

	void Add(Event<Args...> *s){ v = s; }
	void Remove(){ v = nullptr; }

	virtual void Call(Args... args) = 0;

	virtual void *GetEvent() override { return (void *)v; }

	Event<Args...>* v;
};

template<class T, class... Args> class ConcreteCallback : public AbstractCallback < Args... >
{
public:
	ConcreteCallback(Object *sender, T *t, void(T::*f)(Args...), Event<Args...> &s);
	virtual ~ConcreteCallback();
private:
	ConcreteCallback(const ConcreteCallback&);
	void operator=(const ConcreteCallback&) = delete;

	friend class Event < Args... > ;

	virtual void Call(Args... args){ (t->*f)(args...); }

	T *t;
	void(T::*f)(Args...);
};

template<class... Args> class Event
{
public:
	Event(){ }
	~Event(){ for (auto i : v) i.second->Remove(); }

	void Connect(Object *sender, AbstractCallback<Args...> &s){ v.push_back(std::make_pair(sender, &s)); s.Add(this); }
	void Disconnect(AbstractCallback<Args...> &s)
	{ 
		auto newEnd = std::remove_if(v.begin(), v.end(), [&s](const std::pair<Object *, AbstractCallback<Args...>*> &i) { return i.second == &s; });
		v.erase(newEnd, v.end());
	}

	void operator()(Object *sender, Args... args)
	{ 
		auto tmpV = v;
		for (auto i : tmpV)
		{
			if ((i.first == nullptr || i.first == sender) && std::find(v.begin(), v.end(), i) != v.end())
			{
				i.second->Call(args...);
			}
		}
	}

private:
	Event(const Event&);
	void operator=(const Event&) = delete;

	std::vector<std::pair<Object *, AbstractCallback<Args...>*>> v;
};

template<class T, class... Args> ConcreteCallback<T, Args...>::ConcreteCallback(Object *sender, T *t, void(T::*f)(Args...), Event<Args...> &s) : t(t), f(f)
{
	s.Connect(sender, *this);
}

template<class T, class... Args> ConcreteCallback<T, Args...>::~ConcreteCallback()
{
	if (v)
	{
		v->Disconnect(*this);
	}
}

class Callback
{
public:
	Callback(){ }
	~Callback(){ for (auto i : v) delete i; }

	template<class T, class... Args> void Connect(Object *sender, T *t, void(T::*f)(Args...), Event<Args...> &s){ v.push_back(new ConcreteCallback<T, Args...>(sender, t, f, s)); }
	template<class... Args> void Disconnect(Event<Args...> &s) 
	{ 
		for (auto i = v.begin(); i != v.end(); ++i)
		{
			if ((*i)->GetEvent() == (void*)&s)
			{
				BaseCallback *c = *i;
				v.erase(i);
				delete c;
				return;
			}
		}
	}

private:
	Callback(const Callback&);
	void operator=(const Callback&) = delete;

	std::vector<BaseCallback*> v;
};

}