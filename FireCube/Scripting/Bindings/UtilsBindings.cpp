#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Utils/StringHash.h"
#include "Application/Input.h"

using namespace FireCube;

void LuaBindings::InitUtils(sol::state &luaState)
{
	luaState.new_usertype<StringHash>("StringHash",
		sol::constructors<StringHash(const std::string &)>());
	
	luaState.new_usertype<MappedInput>("MappedInput",
		"IsStateOn", &MappedInput::IsStateOn,
		"IsActionTriggered", &MappedInput::IsActionTriggered,
		"GetValue", &MappedInput::GetValue);
}