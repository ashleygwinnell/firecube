#include "Scripting/LuaState.h"
#include "Scripting/LuaFunction.h"
#include "Utils/Logger.h"
#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Scripting/LuaFile.h"

using namespace FireCube;
using namespace luabridge;

LuaState::LuaState(Engine *engine) : Object(engine)
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	static const struct luaL_Reg reg[] =
	{
		{ "print", &LuaState::Print },
		{ NULL, NULL }
	};

	lua_atpanic(luaState, &LuaState::AtPanic);
	
	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, reg, 0);	
	lua_pop(luaState, 1);

	LuaBindings::Init(luaState, engine);	
}

LuaState::~LuaState()
{
	for (auto &f : functions)
	{
		delete f.second;
	}
	lua_close(luaState);
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

int LuaState::AtPanic(lua_State* L)
{
	std::string errorMessage = luaL_checkstring(L, -1);
	LOGERROR("Lua error: error message = '", errorMessage, "'");
	lua_pop(L, 1);
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

lua_State *LuaState::GetState()
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

	auto fields = Split(functionName, '.');
	LuaRef ref = getGlobal(luaState, fields[0].c_str());

	if (fields.size() > 1)
	{
		unsigned int i = 1;
		while (i < fields.size())
		{
			if (ref.isTable() == false)
			{
				LOGERROR(fields[i - 1], " is not a table, when getting function: ", functionName);
				return nullptr;
			}
			LuaRef fieldRef = ref[fields[i++]];
			ref = fieldRef;
		}
	}

	if (ref.isFunction() == false)
	{
		LOGERROR(fields.back(), " is not a function, when getting function: ", functionName);
		return nullptr;
	}
	
	LuaFunction *ret = new LuaFunction(ref);
	functions[functionNameHash] = ret;
	return ret;
}