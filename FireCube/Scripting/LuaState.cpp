#include "Scripting/LuaState.h"
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
	
	lua_getglobal(luaState, "_G");
	luaL_setfuncs(luaState, reg, 0);	
	lua_pop(luaState, 1);

	LuaBindings::InitMath(luaState);
	LuaBindings::InitScene(luaState);
}

LuaState::~LuaState()
{
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