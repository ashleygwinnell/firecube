#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Utils/StringHash.h"
#include "Application/Input.h"

using namespace FireCube;
using namespace LuaIntf;

void LuaBindings::InitUtils(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginClass<StringHash>("StringHash")
			.addConstructor(LUA_ARGS(const std::string &))
		.endClass()
		.beginClass<MappedInput>("MappedInput")
			.addFunction("IsStateOn", &MappedInput::IsStateOn)
			.addFunction("IsActionTriggered", &MappedInput::IsActionTriggered)
			.addFunction("GetValue", &MappedInput::GetValue)
			.addFunction("HasValue", &MappedInput::HasValue)
		.endClass();
}