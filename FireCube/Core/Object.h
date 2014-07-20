#pragma once

#include "Utils/StringHash.h"
#include "Core/Memory.h"

namespace FireCube
{

class Engine;

class Object : public RefCounted
{
public:
	Object(Engine *engine);
	virtual ~Object() {};
	
	//virtual StringHash GetType() const = 0;	
	//virtual const std::string& GetTypeName() const = 0;
protected:
	Engine *engine;
};

/*
#define OBJECT(typeName) \
public: \
	virtual StringHash GetType() const { return GetTypeStatic(); } \
	virtual const std::string& GetTypeName() const { return GetTypeNameStatic(); } \
	static StringHash GetTypeStatic() { static const StringHash typeStatic(#typeName); return typeStatic; } \
	static const std::string& GetTypeNameStatic() { static const std::string typeNameStatic(#typeName); return typeNameStatic; } \
	*/

}