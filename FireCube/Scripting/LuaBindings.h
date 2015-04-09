#pragma once

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
	static void InitRenderer(lua_State *luaState);
	static void InitEngine(lua_State *luaState, Engine *engine);
};

}