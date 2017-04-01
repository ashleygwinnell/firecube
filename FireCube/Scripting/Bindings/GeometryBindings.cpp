#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Geometry/Geometry.h"
#include "Geometry/GeometryGenerator.h"
#include "Geometry/Mesh.h"
#include "Geometry/CollisionQuery.h"
#include "Geometry/Mesh.h"
#include "Scene/Renderable.h"

using namespace FireCube;


void LuaBindings::InitGeometry(sol::state &luaState)
{
	luaState.new_usertype<Geometry>("Geometry");
	
	sol::table GeometryGeneratorTable = luaState.create_named_table("GeometryGenerator");
	GeometryGeneratorTable.set("GenerateBox", &GeometryGenerator::GenerateBox);
	GeometryGeneratorTable.set("GeneratePlane", &GeometryGenerator::GeneratePlane);
	GeometryGeneratorTable.set("GenerateSphere", &GeometryGenerator::GenerateSphere);
	
	luaState.new_usertype<Mesh>("Mesh",
		sol::constructors<Mesh(Engine *)>(),
		"AddGeometry", &Mesh::AddGeometry,
		"boundingBox", sol::property(&Mesh::GetBoundingBox),
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());
	
	luaState.new_usertype<RayQueryResult>("RayQueryResult",
		"distance", &RayQueryResult::distance,
		"normal", &RayQueryResult::normal,
		"renderable", &RayQueryResult::renderable);
	
	luaState.new_usertype<RayQuery>("RayQuery",
		sol::constructors<RayQuery(const Ray &, float)>(),
		"results", &RayQuery::results,
		"ray", &RayQuery::ray);

	sol::table PrimitiveTypeTable = luaState.create_named_table("PrimitiveType");
	PrimitiveTypeTable["LINE_LOOP"] = static_cast<unsigned int>(PrimitiveType::LINE_LOOP);
	PrimitiveTypeTable["LINE_STRIP"] = static_cast<unsigned int>(PrimitiveType::LINE_STRIP);
	PrimitiveTypeTable["LINES"] = static_cast<unsigned int>(PrimitiveType::LINES);
	PrimitiveTypeTable["POINTS"] = static_cast<unsigned int>(PrimitiveType::POINTS);
	PrimitiveTypeTable["QUADS"] = static_cast<unsigned int>(PrimitiveType::QUADS);
	PrimitiveTypeTable["TRIANGLE_FAN"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_FAN);
	PrimitiveTypeTable["TRIANGLE_STRIP"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_STRIP);
	PrimitiveTypeTable["TRIANGLES"] = static_cast<unsigned int>(PrimitiveType::TRIANGLES);
}

