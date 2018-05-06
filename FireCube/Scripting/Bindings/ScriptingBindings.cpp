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

	sol::table ScriptPropertyTypeTable = luaState.create_named_table("ScriptPropertyType");
	ScriptPropertyTypeTable["STRING"] = static_cast<unsigned int>(ScriptPropertyType::STRING);
	ScriptPropertyTypeTable["BOOL"] = static_cast<unsigned int>(ScriptPropertyType::BOOL);
	ScriptPropertyTypeTable["FLOAT"] = static_cast<unsigned int>(ScriptPropertyType::FLOAT);
	ScriptPropertyTypeTable["VEC2"] = static_cast<unsigned int>(ScriptPropertyType::VEC2);
	ScriptPropertyTypeTable["VEC3"] = static_cast<unsigned int>(ScriptPropertyType::VEC3);
	ScriptPropertyTypeTable["VEC4"] = static_cast<unsigned int>(ScriptPropertyType::VEC4);
	ScriptPropertyTypeTable["RGB"] = static_cast<unsigned int>(ScriptPropertyType::RGB);
	ScriptPropertyTypeTable["RGBA"] = static_cast<unsigned int>(ScriptPropertyType::RGBA);
}
