#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"
#include "Physics/PhysicsCollisionQuery.h"

using namespace FireCube;

void LuaBindings::InitPhysics(sol::state &luaState)
{
	luaState.new_usertype<PhysicsWorld>("PhysicsWorld",
		"IntersectRay", &PhysicsWorld::IntersectRay);

	luaState.new_usertype<CharacterController>("CharacterController",
		"velocity", sol::property(&CharacterController::GetVelocity, &CharacterController::SetVelocity),
		"onGround", sol::property(&CharacterController::IsOnGround),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());
	luaState.new_usertype<CollisionShape>("CollisionShape",
		"SetBox", &CollisionShape::SetBox,
		"SetPlane", &CollisionShape::SetPlane,
		"isTrigger", sol::property(&CollisionShape::IsTrigger, &CollisionShape::SetIsTrigger),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());

	luaState.new_usertype<PhysicsRayQueryResult>("PhysicsRayQueryResult",
		"distance", &PhysicsRayQueryResult::distance,
		"normal", &PhysicsRayQueryResult::normal,
		"shape", &PhysicsRayQueryResult::shape);

	luaState.new_usertype<PhysicsRayQuery>("PhysicsRayQuery",
		sol::constructors<PhysicsRayQuery(const Ray &, float)>(),
		"results", &PhysicsRayQuery::results,
		"ray", &PhysicsRayQuery::ray);
}

