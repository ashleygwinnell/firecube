#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Scripting/LuaFile.h"
#include "Scripting/LuaScript.h"

using namespace FireCube;
using namespace LuaIntf;

void LuaBindings::InitScripting(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginExtendClass<LuaFile, Resource>("LuaFile")
		.endClass()
		.beginExtendClass <LuaScript, Component>("LuaScript")
			.addFunction("SubscribeToEvent", &LuaScript::SubscribeToEventFromLua)
			.addFunction("CreateObject", (void(LuaScript::*)(LuaFile *, const std::string &)) &LuaScript::CreateObject)
			.addProperty("objectName", &LuaScript::GetObjectName)
		.endClass();
}
