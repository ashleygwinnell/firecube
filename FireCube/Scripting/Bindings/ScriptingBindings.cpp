#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Scripting/LuaFile.h"
#include "Scripting/LuaScript.h"

using namespace FireCube;

void LuaBindings::InitScripting(sol::state &luaState)
{
	luaState.new_usertype<LuaFile>("LuaFile",
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	luaState.new_usertype<LuaScript>("LuaScript",
		"SubscribeToEvent", &LuaScript::SubscribeToEventFromLua,
		"CreateObject", sol::resolve<void(LuaFile *, const std::string &)>(&LuaScript::CreateObject),		
		"objectName", sol::property(&LuaScript::GetObjectName),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());	
}
