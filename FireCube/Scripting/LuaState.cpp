#include "Scripting/LuaState.h"
#include "Scripting/LuaFunction.h"
#include "Utils/Logger.h"
#include "lua.hpp"
#include "Scripting/LuaBindings.h"
#include "Scripting/LuaFile.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Utils/Filesystem.h"

using namespace FireCube;

LuaState::LuaState(Engine *engine) : Object(engine)
{
	luaState.open_libraries();

	static const struct luaL_Reg reg[] =
	{
		{ "print", &LuaState::Print },
		{ NULL, NULL }
	};	
	
	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, reg, 0);	
	lua_pop(luaState, 1);

	ExecuteFile(engine->GetResourceCache()->GetResource<LuaFile>("Scripts/core.lua"));

	LuaBindings::Init(luaState, engine);	

	originalPackagePath = luaState["package"]["path"];
	
	if (Filesystem::GetAssetsFolder().empty() == false)
	{
		AddPackagePath(Filesystem::GetAssetsFolder() + "/Scripts");
	}
}

LuaState::~LuaState()
{
	functions.clear();
}

int LuaState::Print(lua_State *L)
{
	std::string string;
	int count = lua_gettop(L);
	lua_getglobal(L, "tostring");
	for (int i = 1; i <= count; ++i)
	{
		const char *s;
		// push tostring function
		lua_pushvalue(L, -1);
		// push value to print
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		
		s = lua_tostring(L, -1);
		if (s == nullptr)
		{
			return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
		}

		if (i > 1)
			string += "    ";

		string += s;
		// Pop result
		lua_pop(L, 1);
	}

	LOGINFO(string);	

	return 0;
}

void LuaState::ExecuteFile(LuaFile *luaFile)
{
	if (luaFile->HasExecuted())
		return;

	const auto &code = luaFile->GetCode();
	int error = luaL_loadbuffer(luaState, code.data(), code.size(), luaFile->GetFileName().c_str());
	if (error) 
	{
		std::string errorMessage = lua_tostring(luaState, -1);
		LOGERROR("Failed to load lua code ", luaFile->GetFileName(), ": " + errorMessage);
		lua_pop(luaState, 1);
		return;
	}

	int top = lua_gettop(luaState);

	error = lua_pcall(luaState, 0, 0, 0);
	if (error)
	{
		std::string errorMessage = lua_tostring(luaState, -1);
		LOGERROR("Failed to execute lua code ", luaFile->GetFileName(), ": " + errorMessage);
		lua_settop(luaState, top);
		return;
	}

	luaFile->SetExecuted(true);
}

sol::state &LuaState::GetState()
{
	return luaState;
}

LuaFunction *LuaState::GetFunction(const std::string &functionName)
{
	StringHash functionNameHash(functionName);
	auto functionIter = functions.find(functionNameHash);
	if (functionIter != functions.end())
	{
		return functionIter->second;
	}

	auto parts = Split(functionName, '.');
	sol::object obj;
	if (parts.empty() == false)
	{
		obj = luaState[parts.front()];
	}	

	for (auto i = parts.begin() + 1; obj && i != parts.end(); ++i)
	{
		if (obj)
		{
			obj = obj.as<sol::table>()[*i];
		}
	}

	if (obj.is<sol::function>())
	{		
		LuaFunction *ret = new LuaFunction(obj.as<sol::function>());
		functions[functionNameHash] = ret;
		return ret;
	}
	else
	{
		return nullptr;
	}
}

LuaFunction *LuaState::GetFunction(int index)
{	
	sol::stack_function ref(luaState, index);

	if (ref.valid() == false)
	{
		return nullptr;
	}
	else
	{
		LuaFunction *ret = new LuaFunction(ref);
		return ret;
	}
}

void LuaState::AddPackagePath(const std::string &path)
{
	packagePaths.push_back(path);
	
	std::string additionalPaths;
	for (auto &path : packagePaths)
	{
		additionalPaths += ";" + Filesystem::JoinPath(path, "?.lua");
	}

	luaState["package"]["path"] = originalPackagePath + additionalPaths;
}

sol::object LuaState::GetGlobal(const std::string &name)
{
	return luaState[name];
}
