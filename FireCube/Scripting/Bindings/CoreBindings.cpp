#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Core/Object.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/Material.h"
#include "Rendering/Technique.h"
#include "Geometry/Mesh.h"
#include "Scripting/LuaFile.h"
#include "Audio/Sound.h"
#include "Scene/Prefab.h"
#include "Rendering/Renderer.h"
#include "Application/Application.h"

using namespace FireCube;

sol::object GetResource(ResourceCache *resourceCache, const std::string &type, const std::string &path, sol::this_state s)
{
	sol::state_view lua(s);

	if (type == "Material")
	{
		auto resource = resourceCache->GetResource<Material>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Mesh")
	{
		auto resource = resourceCache->GetResource<Mesh>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "LuaFile")
	{
		auto resource = resourceCache->GetResource<LuaFile>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Sound")
	{
		auto resource = resourceCache->GetResource<Sound>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Prefab")
	{
		auto resource = resourceCache->GetResource<Prefab>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Technique")
	{
		auto resource = resourceCache->GetResource<Technique>(path);
		if (resource)
		{
			return sol::object(lua, sol::in_place, resource);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else
	{
		return sol::make_object(s, sol::nil);
	}

	return sol::make_object(s, sol::nil);
}

void LuaBindings::InitCore(sol::state &luaState, Engine *engine)
{
	luaState.new_usertype<Object>("Object",
		sol::base_classes, sol::bases<RefCounted>());
	
	luaState.new_usertype<Engine>("Engine",
		"renderer", sol::property(&Engine::GetRenderer),
		"resourceCache", sol::property(&Engine::GetResourceCache));
	
	luaState.new_usertype<Resource>("Resource",
		sol::base_classes, sol::bases<Object, RefCounted>());
	
	luaState.new_usertype<ResourceCache>("ResourceCache",
		"GetResource", &GetResource);

	luaState.new_usertype<Application>("Application",
		"SetTimeScale", &Application::SetTimeScale);
	
	luaState["engine"] = engine;
}

