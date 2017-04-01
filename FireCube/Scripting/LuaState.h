#pragma once

#include <map>
#include "Utils/utils.h"
#include "Core/Object.h"
#include "Utils/StringHash.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/sol/sol.hpp"

struct lua_State;

namespace FireCube
{

class Engine;
class LuaFile;
class LuaFunction;

/**
* The class encapsulated the Lua state
*/
class FIRECUBE_API LuaState : public Object
{
	FIRECUBE_OBJECT(LuaState)
public:
	LuaState(Engine *engine);
	~LuaState();
	
	/**
	* Executes a Lua script file	
	* @param luaFile The Lua file to execute
	*/
	void ExecuteFile(LuaFile *luaFile);
	sol::state &GetState();
	LuaFunction *GetFunction(const std::string &functionName);
	LuaFunction *GetFunction(int index = -1);
	
	/**
	* Registers a function
	* @param name The name of the function
	* @param function The function to register
	*/
	template <typename FN>
	void AddFunction(const std::string &name, const FN &function)
	{
		luaState.set_function(name, function);
	}
private:
	static int Print(lua_State *L);	
	sol::state luaState;
	std::map<StringHash, SharedPtr<LuaFunction>> functions;
};

}