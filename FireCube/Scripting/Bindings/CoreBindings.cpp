#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Core/Object.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/Material.h"
#include "Geometry/Mesh.h"
#include "Scripting/LuaFile.h"
#include "Audio/Sound.h"

using namespace FireCube;
using namespace LuaIntf;

int GetResource(ResourceCache *resourceCache, lua_State *L, const std::string &type, const std::string &path)
{
	if (type == "Material")
	{
		auto resource = resourceCache->GetResource<Material>(path);
		if (resource)
		{
			Lua::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "Mesh")
	{
		auto resource = resourceCache->GetResource<Mesh>(path);
		if (resource)
		{
			Lua::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "LuaFile")
	{
		auto resource = resourceCache->GetResource<LuaFile>(path);
		if (resource)
		{
			Lua::push(L, resource);
		}
		else
		{
			lua_pushnil(L);
		}
	}
	else if (type == "Sound")
	{
		auto resource = resourceCache->GetResource<Sound>(path);
		if (resource)
		{
			Lua::push(L, resource);
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
	LuaBinding(luaState)
		.beginClass<Object>("Object")
		.endClass()
		.beginClass<Engine>("Engine")
			.addProperty("renderer", &Engine::GetRenderer)
			.addProperty("resourceCache", &Engine::GetResourceCache)
		.endClass()
		.beginClass<Resource>("Resource")
		.endClass()
		.beginClass<ResourceCache>("ResourceCache")
			.addFunction("GetResource", &GetResource)
		.endClass();

	Lua::setGlobal(luaState, "engine", engine);
}

