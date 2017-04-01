#pragma once

#include <map>
#include "Application/Input.h"
#include "Utils/utils.h"
#include "Scripting/LuaFunction.h"
#include "Core/Component.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/Sol/sol.hpp"

namespace FireCube
{

class LuaFile;
class CharacterController;
class CollisionShape;
class LuaBindings;

enum class ScriptFunction
{
	INIT, UPDATE, AWAKE, HANDLE_INPUT, CHARACTER_CONTROLLER_COLLISION
};

/**
* This components is used to create script objects
*/
class FIRECUBE_API LuaScript : public Component
{
	friend class LuaBindings;
	FIRECUBE_OBJECT(LuaScript);
public:
	LuaScript(Engine *engine);
	
	/**
	* Creates a script object
	* @param objectName The name of the object to create
	*/
	void CreateObject(const std::string &objectName);
	
	/**
	* Creates a script object. The function first executes a given lua script file.
	* @param luaFile The Lua script file to execute
	* @param objectName The name of the object to create
	*/
	void CreateObject(LuaFile *luaFile, const std::string &objectName);
	
	/**
	* Returns a free function defined in the script or nullptr if the function wasn't found
	* @param functionName The name of the function to get
	*/ 
	LuaFunction *GetFunction(const std::string &functionName);
	
	/**
	* Returns a member function of the currently created script object or nullptr if the function wasn't found
	* @param functionName The name of the member function to get
	*/
	LuaFunction *GetMemberFunction(const std::string &functionName);
	LuaFile *GetLuaFile();
	
	/**
	* @returns The script object name
	*/
	std::string GetObjectName() const;

	/**
	* Calls a member Lua function
	* @param function The Lua member function to call 
	*/
	template<class... Args>
	void CallMemberFunction(LuaFunction *function, Args&&... args)
	{
		(*function)(object, std::forward<Args>(args)...);
	}

	/**
	* Calls a member Lua function
	* @param functionName The member function's name 
	*/
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

	/**
	* Assign a value to a property of the Lua script object
	* @param name The name of the property
	* @param value The value to assign
	*/
	template<class T>
	void SetProperty(const std::string &name, T value)
	{
		object.set(name, value);
	}

	void PushObject();
	sol::table GetScriptObject();
	
	/**
	* Assign a value to a property. These values are assigned once the object is created
	* @param property The property name
	* @param value The property value
	*/
	void SetInitialPropertyValue(const std::string &property, const std::string &value);
	
	/**
	* Clones this component 
	*/
	virtual Component *Clone() const;
private:

	LuaScript(const LuaScript &other);

	void Update(float dt);
	void HandleInput(float dt, const MappedInput &input);
	void CharacterControllerCollision(CharacterController *characterController, CollisionShape *collisionShape);
	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	void SetInitialProperties();
	void SubscribeToEventFromLua(const std::string &eventName, sol::object param);

	bool awakeCalled;
	LuaFile *luaFile;
	std::string objectName;
	sol::table object;
	std::map<ScriptFunction, LuaFunction *> scriptFunctions;	
	std::map<std::string, std::string> initialProperties;
};

}