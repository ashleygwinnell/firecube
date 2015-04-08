#include "Utils/Logger.h"
#include "Scripting/LuaScript.h"
#include "Scripting/LuaFunction.h"
#include "Core/Events.h"
#include "Core/Engine.h"
#include "Scripting/LuaState.h"

using namespace FireCube;
using namespace luabridge;

LuaScript::LuaScript(Engine *engine) : Component(engine), object(engine->GetLuaState()->GetState()), initFunction(engine->GetLuaState()->GetState()), updateFunction(engine->GetLuaState()->GetState()),
									   handleInputFunction(engine->GetLuaState()->GetState()), awakeFunction(engine->GetLuaState()->GetState())
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
}

LuaScript::LuaScript(const LuaScript &other) : Component(other), objectName(other.objectName), object(engine->GetLuaState()->GetState()), initFunction(engine->GetLuaState()->GetState()), 
											   updateFunction(engine->GetLuaState()->GetState()), handleInputFunction(engine->GetLuaState()->GetState()), awakeFunction(engine->GetLuaState()->GetState())
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
	CreateObject(objectName);

	// TODO: Copy properties assigned to the object through the script
}

void LuaScript::Update(float time)
{	
	if (IsEnabled() && updateFunction.isFunction())
	{
		// Call update function	
		updateFunction(object, time);
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
		if (node && initFunction.isFunction())
		{
			initFunction(object);
		}
	}
}

void LuaScript::SceneChanged(Scene *oldScene)
{
	if (scene && objectName.empty() == false && IsEnabled() && awakeFunction.isFunction())
	{
		// Call awake function	
		awakeFunction(object);
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
		
	object = newTable(state);
	object.push(state);
	objectTable.push(state);
	lua_setmetatable(state, -2);
	lua_pop(state, 1);
	
	object["script"] = this;
	initFunction = objectTable["Init"];
	updateFunction = objectTable["Update"];
	awakeFunction = objectTable["Awake"];
	if (node && initFunction.isFunction())
	{
		object["node"] = node;
		initFunction(object);
	}
	if (scene && awakeFunction.isFunction())
	{
		awakeFunction(object);
	}
	this->objectName = objectName;
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
}

void LuaScript::HandleInput(float time, const MappedInput &input)
{
	handleInputFunction(object, time, input);
}

Component *LuaScript::Clone() const
{
	LuaScript *clone = new LuaScript(*this);
	return clone;
}