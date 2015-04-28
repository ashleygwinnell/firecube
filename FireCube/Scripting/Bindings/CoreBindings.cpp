#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Core/Object.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/Material.h"
#include "Geometry/Mesh.h"
#include "Geometry/AnimatedMesh.h"
#include "Scripting/LuaFile.h"
#include "Audio/Sound.h"

using namespace FireCube;
using namespace luabridge;

int GetResource(lua_State *L)
{
	LuaRef param1 = LuaRef::fromStack(L, -3);
	LuaRef param2 = LuaRef::fromStack(L, -2);
	LuaRef param3 = LuaRef::fromStack(L, -1);
	ResourceCache *resrouceCahce = param1.cast<ResourceCache*>();
	std::string type = param2;
	std::string path = param3;

	if (type == "Material")
	{
		auto resource = resrouceCahce->GetResource<Material>(path);
		if (resource)
		{
			UserdataPtr::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "Mesh")
	{
		auto resource = resrouceCahce->GetResource<Mesh>(path);
		if (resource)
		{
			UserdataPtr::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "LuaFile")
	{
		auto resource = resrouceCahce->GetResource<LuaFile>(path);
		if (resource)
		{
			UserdataPtr::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "Sound")
	{
		auto resource = resrouceCahce->GetResource<Sound>(path);
		if (resource)
		{
			UserdataPtr::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "AnimatedMesh")
	{
		auto resource = resrouceCahce->GetResource<AnimatedMesh>(path);
		if (resource)
		{
			UserdataPtr::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

void LuaBindings::InitCore(lua_State *luaState, Engine *engine)
{
	getGlobalNamespace(luaState)
		.beginClass<Object>("Object")
		.endClass()
		.beginClass<Engine>("Engine")
			.addProperty("renderer", &Engine::GetRenderer)
			.addProperty("resourceCache", &Engine::GetResourceCache)
		.endClass()
		.beginClass<Resource>("Resource")
		.endClass()
		.beginClass<ResourceCache>("ResourceCache")
			.addCFunctionFree("GetResource", &GetResource)
		.endClass();

	luabridge::setGlobal(luaState, engine, "engine");
}

