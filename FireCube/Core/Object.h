#pragma once

#include "Utils/StringHash.h"
#include "Core/Memory.h"
#include "Core/EventSystem.h"

namespace FireCube
{

class Engine;

class Object : public RefCounted
{
public:
	Object(Engine *engine);
	virtual ~Object() {};

	template<class T, class... Args>
	void SubscribeToEvent(Event<Args...> &s, void(T::*f)(Args...))
	{
		eventCallback.Connect(static_cast<T *>(this), f, s);
	}
	
	virtual StringHash GetType() const = 0;	
	virtual const std::string& GetTypeName() const = 0;
protected:
	Engine *engine;
	Callback eventCallback;
};


#define OBJECT(typeName) \
public: \
	virtual FireCube::StringHash GetType() const { return GetTypeStatic(); } \
	virtual const std::string& GetTypeName() const { return GetTypeNameStatic(); } \
	static FireCube::StringHash GetTypeStatic() { static const FireCube::StringHash typeStatic(#typeName); return typeStatic; } \
	static const std::string& GetTypeNameStatic() { static const std::string typeNameStatic(#typeName); return typeNameStatic; } \

}