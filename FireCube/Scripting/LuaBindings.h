#pragma once

struct lua_State;

namespace FireCube
{

class LuaBindings
{
public:
	static void InitMath(lua_State *luaState);
	static void InitScene(lua_State *luaState);
	static void InitUtils(lua_State *luaState);
};

}