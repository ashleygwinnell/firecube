#pragma once

#include <vector>
#include "Geometry/CollisionQuery.h"
#include "Rendering/RenderingTypes.h"
#include "Core/Memory.h"
#include "Scene/Node.h"
#include "Scene/Light.h"

struct lua_State;

namespace FireCube
{

class Engine;

class LuaBindings
{
public:
	static void Init(lua_State *luaState, Engine *engine);
	static void InitMath(lua_State *luaState);
	static void InitScene(lua_State *luaState);
	static void InitUtils(lua_State *luaState);
	static void InitRendering(lua_State *luaState);	
	static void InitCore(lua_State *luaState, Engine *engine);
	static void InitScripting(lua_State *luaState);
	static void InitGeometry(lua_State *luaState);
	static void InitPhysics(lua_State *luaState);
	static void InitAudio(lua_State *luaState);
};

}

namespace luabridge
{

template <>
struct Stack < std::vector<FireCube::RayQueryResult> >
{
	static void push(lua_State* L, const std::vector<FireCube::RayQueryResult> &results)
	{
		LuaRef ret = LuaRef::newTable(L);
		for (auto &result : results)
		{
			ret.append(result);
		}

		ret.push(L);
	}

	static std::vector<FireCube::RayQueryResult> get(lua_State* L, int index)
	{
		return{};
	}
};

template <>
struct Stack < FireCube::PrimitiveType >
{
	static void push(lua_State* L, FireCube::PrimitiveType type)
	{
		lua_pushnumber(L, static_cast<unsigned int>(type));
	}

	static FireCube::PrimitiveType get(lua_State* L, int index)
	{
		return static_cast<FireCube::PrimitiveType>((unsigned int)lua_tonumber(L, index));
	}

};

template <>
struct Stack < FireCube::LightType >
{
	static void push(lua_State* L, FireCube::LightType type)
	{
		lua_pushnumber(L, static_cast<unsigned int>(type));
	}

	static FireCube::LightType get(lua_State* L, int index)
	{
		return static_cast<FireCube::LightType>((unsigned int)lua_tonumber(L, index));
	}

};


template <>
struct Stack < std::vector<FireCube::SharedPtr<FireCube::Node>> >
{
	static void push(lua_State* L, const std::vector<FireCube::SharedPtr<FireCube::Node>> &nodes)
	{
		LuaRef ret = LuaRef::newTable(L);

		for (auto &n : nodes)
		{
			ret.append(n.Get());
		}

		ret.push(L);
	}
};

template <>
struct Stack < std::vector<std::string> >
{
	static void push(lua_State* L, const std::vector<std::string> &strings)
	{
		LuaRef ret = LuaRef::newTable(L);

		for (auto &str : strings)
		{
			ret.append(str);
		}

		ret.push(L);
	}
};


}