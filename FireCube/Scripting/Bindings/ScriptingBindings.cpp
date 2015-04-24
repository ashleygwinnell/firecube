#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Scripting/LuaFile.h"
#include "Scripting/LuaScript.h"

using namespace FireCube;
using namespace luabridge;

void LuaBindings::InitScripting(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.deriveClass<LuaFile, Resource>("LuaFile")
		.endClass()
		.deriveClass <LuaScript, Component>("LuaScript")
			.addFunction("SubscribeToEvent", &LuaScript::SubscribeToEventFromLua)
			.addFunction("CreateObject", (void(LuaScript::*)(LuaFile *, const std::string &)) &LuaScript::CreateObject)
		.endClass();
}
