#pragma once

#include <vector>
#include <algorithm>

namespace FireCube
{

template<class... Args> class Event;

class BaseCallback
{
public:
	virtual ~BaseCallback(){ }
};

template<class... Args> class AbstractCallback : public BaseCallback
{
protected:
	virtual ~AbstractCallback();

	friend class Event < Args... > ;

	void Add(Event<Args...> *s){ v.push_back(s); }
	void Remove(Event<Args...> *s){ v.erase(std::remove(v.begin(), v.end(), s), v.end()); }

	virtual void Call(Args... args) = 0;

	std::vector<Event<Args...>*> v;
};

template<class T, class... Args> class ConcreteCallback : public AbstractCallback < Args... >
{
public:
	ConcreteCallback(T *t, void(T::*f)(Args...), Event<Args...> &s);

private:
	ConcreteCallback(const ConcreteCallback&);
	void operator=(const ConcreteCallback&);

	friend class Event < Args... > ;

	virtual void Call(Args... args){ (t->*f)(args...); }

	T *t;
	void(T::*f)(Args...);
};

template<class... Args> class Event
{
public:
	Event(){ }
	~Event(){ for (auto i : v) i->Remove(this); }

	void Connect(AbstractCallback<Args...> &s){ v.push_back(&s); s.Add(this); }
	void Disconnect(AbstractCallback<Args...> &s){ v.erase(std::remove(v.begin(), v.end(), &s), v.end()); }

	void operator()(Args... args){ for (auto i : v) i->Call(args...); }

private:
	Event(const Event&);
	void operator=(const Event&);

	std::vector<AbstractCallback<Args...>*> v;
};

template<class... Args> AbstractCallback<Args...>::~AbstractCallback()
{
	for (auto i : v) i->Disconnect(*this);
}

template<class T, class... Args> ConcreteCallback<T, Args...>::ConcreteCallback(T *t, void(T::*f)(Args...), Event<Args...> &s) : t(t), f(f)
{
	s.Connect(*this);
}

class Callback
{
public:
	Callback(){ }
	~Callback(){ for (auto i : v) delete i; }

	template<class T, class... Args> void Connect(T *t, void(T::*f)(Args...), Event<Args...> &s){ v.push_back(new ConcreteCallback<T, Args...>(t, f, s)); }

private:
	Callback(const Callback&);
	void operator=(const Callback&);

	std::vector<BaseCallback*> v;
};

}