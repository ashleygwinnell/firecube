#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"

using namespace FireCube;
using namespace luabridge;

void LuaBindings::InitPhysics(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.deriveClass<CharacterController, Component>("CharacterController")
			.addProperty("velocity", &CharacterController::GetVelocity, &CharacterController::SetVelocity)
			.addProperty("onGround", &CharacterController::IsOnGround)
		.endClass()
		.deriveClass<CollisionShape, Component>("CollisionShape")
			.addFunction("SetBox", &CollisionShape::SetBox)
			.addFunction("SetPlane", &CollisionShape::SetPlane)
			.addProperty("isTrigger", &CollisionShape::IsTrigger, &CollisionShape::SetIsTrigger)
		.endClass();
}

