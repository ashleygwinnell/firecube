#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"

using namespace FireCube;

void LuaBindings::InitPhysics(sol::state &luaState)
{
	luaState.new_usertype<CharacterController>("CharacterController",
		"velocity", sol::property(&CharacterController::GetVelocity, &CharacterController::SetVelocity),
		"onGround", sol::property(&CharacterController::IsOnGround),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());
	luaState.new_usertype<CollisionShape>("CollisionShape",
		"SetBox", &CollisionShape::SetBox,
		"SetPlane", &CollisionShape::SetPlane,
		"isTrigger", sol::property(&CollisionShape::IsTrigger, &CollisionShape::SetIsTrigger),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());
}

