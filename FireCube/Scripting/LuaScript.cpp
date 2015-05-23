#include "Utils/Logger.h"
#include "Scripting/LuaScript.h"
#include "Scripting/LuaFunction.h"
#include "Core/Events.h"
#include "Core/Engine.h"
#include "Scripting/LuaState.h"

using namespace FireCube;
using namespace luabridge;

LuaScript::LuaScript(Engine *engine) : Component(engine), object(engine->GetLuaState()->GetState()), luaFile(nullptr), awakeCalled(false)
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
}

LuaScript::LuaScript(const LuaScript &other) : Component(other), objectName(other.objectName), object(engine->GetLuaState()->GetState()), luaFile(other.luaFile), awakeCalled(false)
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
	CreateObject(objectName);

	// TODO: Copy properties assigned to the object through the script
}

void LuaScript::Update(float time)
{	
	if (awakeCalled == false)
	{		
		awakeCalled = true;

		auto awakeFunction = scriptFunctions[ScriptFunction::AWAKE];
		if (scene && objectName.empty() == false && IsEnabled() && awakeFunction)
		{				
			// Call awake function	
			try
			{
				(*awakeFunction)(object);
			}
			catch (LuaException &e)
			{
				(void)e; // Disable warning about e not being used
				LOGERROR(e.what());
			}
		}
	}

	auto updateFunction = scriptFunctions[ScriptFunction::UPDATE];
	if (awakeCalled && IsEnabled() && updateFunction)
	{
		// Call update function	
		try
		{
			(*updateFunction)(object, time);
		}
		catch (LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}		
	}
}

void LuaScript::MarkedDirty()
{

}

void LuaScript::NodeChanged()
{	
	if (object.isTable())
	{
		object["node"] = node;
		auto initFunction = scriptFunctions[ScriptFunction::INIT];
		if (node && initFunction)
		{		
			try
			{
				(*initFunction)(object);
			}
			catch (LuaException &e)
			{
				(void)e; // Disable warning about e not being used
				LOGERROR(e.what());
			}
		}
	}
}

void LuaScript::SceneChanged(Scene *oldScene)
{
	
}

void LuaScript::CreateObject(const std::string &objectName)
{	
	lua_State *state = engine->GetLuaState()->GetState();
	LuaRef objectTable = getGlobal(state, objectName.c_str());
	if (objectTable.isTable() == false)
	{
		LOGERROR("Can't init script object: ", objectName, " is not a table");
		return;
	}
		
	this->objectName = objectName;
	object = newTable(state);
	object.push(state);
	LuaRef metatable = newTable(state);
	metatable["__index"] = objectTable;
	metatable.push(state);
	lua_setmetatable(state, -2);
	lua_pop(state, 1);
	
	object["script"] = this;
	scriptFunctions[ScriptFunction::INIT] = GetMemberFunction("Init");	
	scriptFunctions[ScriptFunction::UPDATE] = GetMemberFunction("Update");	
	scriptFunctions[ScriptFunction::AWAKE] = GetMemberFunction("Awake");
	
	auto initFunction = scriptFunctions[ScriptFunction::INIT];
	if (node && initFunction)
	{
		object["node"] = node;		
		try
		{
			(*initFunction)(object);
		}
		catch (LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}
	}		
}

void LuaScript::CreateObject(LuaFile *luaFile, const std::string &objectName)
{
	this->luaFile = luaFile;
	engine->GetLuaState()->ExecuteFile(luaFile);
	CreateObject(objectName);
}

LuaFunction *LuaScript::GetFunction(const std::string &functionName)
{
	return engine->GetLuaState()->GetFunction(functionName);
}

LuaFunction *LuaScript::GetMemberFunction(const std::string &functionName)
{
	return GetFunction(objectName + "." + functionName);
}

void LuaScript::SubscribeToEventFromLua(const std::string &eventName, LuaRef param)
{
	Object *sender = nullptr;
	int functionIndex = -1;
	if (param.isFunction() == false)
	{
		sender = param.cast<Object *>();
	}	

	if (eventName == "HandleInput")
	{
		scriptFunctions[ScriptFunction::HANDLE_INPUT] = engine->GetLuaState()->GetFunction();		
		SubscribeToEvent(sender, Events::HandleInput, &LuaScript::HandleInput);
	}
	else if (eventName == "CharacterControllerCollision")
	{
		scriptFunctions[ScriptFunction::CHARACTER_CONTROLLER_COLLISION] = engine->GetLuaState()->GetFunction();
		SubscribeToEvent(sender, Events::CharacterControllerCollision, &LuaScript::CharacterControllerCollision);
	}
}

void LuaScript::HandleInput(float time, const MappedInput &input)
{
	if (awakeCalled)
	{
		try
		{
			(*scriptFunctions[ScriptFunction::HANDLE_INPUT])(object, time, input);
		}
		catch (LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}
	}
}

void LuaScript::CharacterControllerCollision(CharacterController *characterController, CollisionShape *collisionShape)
{
	if (awakeCalled)
	{
		try
		{
			(*scriptFunctions[ScriptFunction::CHARACTER_CONTROLLER_COLLISION])(object, characterController, collisionShape);
		}
		catch (LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}
	}
}

LuaFile *LuaScript::GetLuaFile()
{
	return luaFile;
}

std::string LuaScript::GetObjectName() const
{
	return objectName;
}

Component *LuaScript::Clone() const
{
	LuaScript *clone = new LuaScript(*this);
	return clone;
}
