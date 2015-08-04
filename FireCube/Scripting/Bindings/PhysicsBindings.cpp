#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"

using namespace FireCube;
using namespace LuaIntf;

void LuaBindings::InitPhysics(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginExtendClass<CharacterController, Component>("CharacterController")
			.addProperty("velocity", &CharacterController::GetVelocity, &CharacterController::SetVelocity)
			.addProperty("onGround", &CharacterController::IsOnGround)
		.endClass()
		.beginExtendClass<CollisionShape, Component>("CollisionShape")
			.addFunction("SetBox", &CollisionShape::SetBox)
			.addFunction("SetPlane", &CollisionShape::SetPlane)
			.addProperty("isTrigger", &CollisionShape::IsTrigger, &CollisionShape::SetIsTrigger)
		.endClass();
}

