#pragma once

#include "Utils/utils.h"
#include "Core/Object.h"

struct lua_State;

namespace FireCube
{

class Engine;
class LuaFile;

class FIRECUBE_API LuaState : public Object
{
	OBJECT(LuaState)
public:
	LuaState(Engine *engine);
	~LuaState();

	void ExecuteFile(LuaFile *luaFile);
	lua_State *GetState();
private:
	static int Print(lua_State *L);
	lua_State *luaState;
};

}