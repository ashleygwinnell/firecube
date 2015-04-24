#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Utils/StringHash.h"
#include "Application/Input.h"

using namespace FireCube;
using namespace luabridge;

void LuaBindings::InitUtils(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<StringHash>("StringHash")
			.addConstructor<void(*) (const std::string &)>()
		.endClass()
		.beginClass<MappedInput>("MappedInput")
			.addFunction("IsStateOn", &MappedInput::IsStateOn)
			.addFunction("IsActionTriggered", &MappedInput::IsActionTriggered)
			.addFunction("GetValue", &MappedInput::GetValue)
			.addFunction("HasValue", &MappedInput::HasValue)
		.endClass();
}