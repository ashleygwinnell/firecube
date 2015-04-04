#pragma once

#include "Application/Input.h"
#include "Utils/utils.h"
#include "Scripting/LuaFunction.h"
#include "Core/Component.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"

namespace FireCube
{

class LuaFile;

class FIRECUBE_API LuaScript : public Component
{
	OBJECT(LuaScript);
public:
	LuaScript(Engine *engine);
	void CreateObject(const std::string &objectName);
	void CreateObject(LuaFile *luaFile, const std::string &objectName);
	LuaFunction *GetFunction(const std::string &functionName);
	LuaFunction *GetMemberFunction(const std::string &functionName);
	void SubscribeToEventFromLua(const std::string &eventName, luabridge::LuaRef function);
	void HandleInput(float time, const MappedInput &input);

	template<class... Args>
	void CallMemberFunction(LuaFunction *function, Args&&... args)
	{
		(*function)(object, std::forward<Args>(args)...);
	}

	template<class... Args>
	void CallMemberFunction(const std::string &functionName, Args&&... args)
	{
		auto function = GetMemberFunction(functionName);
		if (function)
		{
			(*function)(object, std::forward<Args>(args)...);
		}
	}

	template<class... Args>
	void CallFunction(LuaFunction *function, Args&&... args)
	{
		(*function)(std::forward<Args>(args)...);
	}

	template<class... Args>
	void CallFunction(const std::string &functionName, Args&&... args)
	{
		auto function = GetFunction(functionName);
		if (function)
		{
			(*function)(std::forward<Args>(args)...);
		}
	}
	
private:
	void Update(float time);
	
	virtual void MarkedDirty();
	virtual void NodeChanged();

	std::string objectName;
	luabridge::LuaRef object;
	luabridge::LuaRef initFunction, updateFunction, handleInputFunction;
};

}