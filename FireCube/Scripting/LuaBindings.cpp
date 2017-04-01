#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"

using namespace FireCube;

void LuaBindings::Init(sol::state &luaState, Engine *engine)
{
	LuaBindings::InitMath(luaState);
	LuaBindings::InitCore(luaState, engine);		
	LuaBindings::InitScene(luaState);
	LuaBindings::InitUtils(luaState);
	LuaBindings::InitRendering(luaState);	
	LuaBindings::InitScripting(luaState);
	LuaBindings::InitGeometry(luaState);
	LuaBindings::InitPhysics(luaState);
	LuaBindings::InitAudio(luaState);

}

