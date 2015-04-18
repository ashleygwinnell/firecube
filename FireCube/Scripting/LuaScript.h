#pragma once

#include <map>
#include "Application/Input.h"
#include "Utils/utils.h"
#include "Scripting/LuaFunction.h"
#include "Core/Component.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"

namespace FireCube
{

class LuaFile;
class CharacterController;
class CollisionShape;

enum class ScriptFunction
{
	INIT, UPDATE, AWAKE
};

class FIRECUBE_API LuaScript : public Component
{
	OBJECT(LuaScript);
public:
	LuaScript(Engine *engine);
	void CreateObject(const std::string &objectName);
	void CreateObject(LuaFile *luaFile, const std::string &objectName);
	LuaFunction *GetFunction(const std::string &functionName);
	LuaFunction *GetMemberFunction(const std::string &functionName);
	void SubscribeToEventFromLua(const std::string &eventName, luabridge::LuaRef param1, luabridge::LuaRef param2);

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
	
	virtual Component *Clone() const;
private:

	LuaScript(const LuaScript &other);

	void Update(float time);
	void HandleInput(float time, const MappedInput &input);
	void CharacterControllerCollision(CharacterController *characterController, CollisionShape *collisionShape);
	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);

	std::string objectName;
	luabridge::LuaRef object;
	std::map<ScriptFunction, LuaFunction *> scriptFunctions;
	luabridge::LuaRef handleInputFunction, characterControllerCollisionFunction;
};

}