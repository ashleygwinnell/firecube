#pragma once

#include <map>
#include "Utils/utils.h"
#include "Core/Object.h"
#include "Utils/StringHash.h"

struct lua_State;

namespace FireCube
{

class Engine;
class LuaFile;
class LuaFunction;

class FIRECUBE_API LuaState : public Object
{
	OBJECT(LuaState)
public:
	LuaState(Engine *engine);
	~LuaState();

	void ExecuteFile(LuaFile *luaFile);
	lua_State *GetState();
	LuaFunction *GetFunction(const std::string &functionName);
	LuaFunction *GetFunction(int index = -1);
private:
	static int Print(lua_State *L);
	static int LuaState::AtPanic(lua_State* L);
	lua_State *luaState;
	std::map<StringHash, SharedPtr<LuaFunction>> functions;
};

}