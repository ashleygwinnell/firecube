#include "Utils/Logger.h"
#include "Scripting/LuaScript.h"
#include "Scripting/LuaFunction.h"
#include "Core/Events.h"
#include "Core/Engine.h"
#include "Scripting/LuaState.h"

using namespace FireCube;
using namespace luabridge;

LuaScript::LuaScript(Engine *engine) : Component(engine), object(engine->GetLuaState()->GetState()), handleInputFunction(engine->GetLuaState()->GetState()), 
									   characterControllerCollisionFunction(engine->GetLuaState()->GetState())
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
}

LuaScript::LuaScript(const LuaScript &other) : Component(other), objectName(other.objectName), object(engine->GetLuaState()->GetState()), handleInputFunction(engine->GetLuaState()->GetState()),
											   characterControllerCollisionFunction(engine->GetLuaState()->GetState())
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
	CreateObject(objectName);

	// TODO: Copy properties assigned to the object through the script
}

void LuaScript::Update(float time)
{	
	auto updateFunction = scriptFunctions[ScriptFunction::UPDATE];
	if (IsEnabled() && updateFunction)
	{
		// Call update function	
		(*updateFunction)(object, time);
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
			(*initFunction)(object);
		}
	}
}

void LuaScript::SceneChanged(Scene *oldScene)
{
	auto awakeFunction = scriptFunctions[ScriptFunction::AWAKE];
	if (scene && objectName.empty() == false && IsEnabled() && awakeFunction)
	{
		// Call awake function	
		(*awakeFunction)(object);
	}
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
		(*initFunction)(object);
	}

	auto awakeFunction = scriptFunctions[ScriptFunction::AWAKE];
	if (scene && awakeFunction)
	{
		(*awakeFunction)(object);
	}
	
}

void LuaScript::CreateObject(LuaFile *luaFile, const std::string &objectName)
{
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

void LuaScript::SubscribeToEventFromLua(const std::string &eventName, LuaRef function)
{
	if (eventName == "HandleInput")
	{
		handleInputFunction = function;
		SubscribeToEvent(Events::HandleInput, &LuaScript::HandleInput);
	}
	else if (eventName == "CharacterControllerCollision")
	{
		characterControllerCollisionFunction = function;
		SubscribeToEvent(Events::CharacterControllerCollision, &LuaScript::CharacterControllerCollision);
	}
}

void LuaScript::HandleInput(float time, const MappedInput &input)
{
	handleInputFunction(object, time, input);
}

void LuaScript::CharacterControllerCollision(CharacterController *characterController, CollisionShape *collisionShape)
{
	characterControllerCollisionFunction(object, characterController, collisionShape);
}

Component *LuaScript::Clone() const
{
	LuaScript *clone = new LuaScript(*this);
	return clone;
}