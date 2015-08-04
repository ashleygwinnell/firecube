#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Geometry/Geometry.h"
#include "Geometry/GeometryGenerator.h"
#include "Geometry/Mesh.h"
#include "Geometry/CollisionQuery.h"
#include "Geometry/Mesh.h"

using namespace FireCube;
using namespace LuaIntf;



void LuaBindings::InitGeometry(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginClass<Geometry>("Geometry")
		.endClass()
		.beginModule("GeometryGenerator")
			.addFunction("GenerateBox", &GeometryGenerator::GenerateBox)
			.addFunction("GeneratePlane", &GeometryGenerator::GeneratePlane)
			.addFunction("GenerateSphere", &GeometryGenerator::GenerateSphere)
		.endModule()		
		.beginExtendClass<Mesh, Resource>("Mesh")
			.addConstructor(LUA_ARGS(Engine *))
			.addFunction("AddGeometry", &Mesh::AddGeometry)
			.addProperty("boundingBox", &Mesh::GetBoundingBox)
		.endClass()
		.beginClass<RayQueryResult>("RayQueryResult")
			.addVariable("distance", &RayQueryResult::distance)
			.addVariable("normal", &RayQueryResult::normal)
			.addVariable("renderable", &RayQueryResult::renderable)
		.endClass()
		.beginClass<RayQuery>("RayQuery")
			.addConstructor(LUA_ARGS(const Ray &, float))
			.addVariable("results", &RayQuery::results, false)
			.addVariable("ray", &RayQuery::ray)
		.endClass();

	LuaRef t = LuaRef::createTable(luaState);
	t["LINE_LOOP"] = static_cast<unsigned int>(PrimitiveType::LINE_LOOP);
	t["LINE_STRIP"] = static_cast<unsigned int>(PrimitiveType::LINE_STRIP);
	t["LINES"] = static_cast<unsigned int>(PrimitiveType::LINES);
	t["POINTS"] = static_cast<unsigned int>(PrimitiveType::POINTS);
	t["QUADS"] = static_cast<unsigned int>(PrimitiveType::QUADS);
	t["TRIANGLE_FAN"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_FAN);
	t["TRIANGLE_STRIP"] = static_cast<unsigned int>(PrimitiveType::TRIANGLE_STRIP);
	t["TRIANGLES"] = static_cast<unsigned int>(PrimitiveType::TRIANGLES);
	Lua::setGlobal(luaState, "PrimitiveType", t);
}

