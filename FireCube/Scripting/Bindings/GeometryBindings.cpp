#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Geometry/Geometry.h"
#include "Geometry/GeometryGenerator.h"
#include "Geometry/Mesh.h"
#include "Geometry/CollisionQuery.h"
#include "Geometry/Mesh.h"

using namespace FireCube;
using namespace luabridge;



void LuaBindings::InitGeometry(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Geometry>("Geometry")
		.endClass()
		.beginNamespace("GeometryGenerator")
			.addFunction("GenerateBox", &GeometryGenerator::GenerateBox)
			.addFunction("GeneratePlane", &GeometryGenerator::GeneratePlane)
			.addFunction("GenerateSphere", &GeometryGenerator::GenerateSphere)
		.endNamespace()		
		.deriveClass<Mesh, Resource>("Mesh")
			.addConstructor<void(*) (Engine *)>()			
			.addFunction("AddGeometry", &Mesh::AddGeometry)
		.endClass()
		.beginClass<RayQueryResult>("RayQueryResult")
			.addData("distance", &RayQueryResult::distance)
			.addData("normal", &RayQueryResult::normal)
			.addData("renderable", &RayQueryResult::renderable)
		.endClass()
		.beginClass<RayQuery>("RayQuery")
			.addConstructor<void(*) (const Ray &, float)>()
			.addData("results", &RayQuery::results, false)
			.addData("ray", &RayQuery::ray)
		.endClass();

	LuaRef t = LuaRef::newTable(luaState);
	t["LINE_LOOP"] = static_cast<unsigned int>(PrimitiveType::LINE_LOOP);
	t["LINE_STRIP"] = static_cast<unsigned int>(PrimitiveType::LINE_STRIP);
	t["LINES"] = static_cast<unsigned int>(PrimitiveType::LINES);
	t["POINTS"] = static_cast<unsigned int>(PrimitiveType::POINTS);
	t["QUADS"] = static_cast<unsigned int>(PrimitiveType::QUADS);
	t["TRIANGLE_FAN"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_FAN);
	t["TRIANGLE_STRIP"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_STRIP);
	t["TRIANGLES"] = static_cast<unsigned int>(PrimitiveType::TRIANGLES);
	setGlobal(luaState, t, "PrimitiveType");
}

