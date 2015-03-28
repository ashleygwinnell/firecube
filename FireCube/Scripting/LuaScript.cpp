#include "Utils/Logger.h"
#include "Scripting/LuaScript.h"
#include "Core/Events.h"
#include "Core/Engine.h"
#include "Scripting/LuaState.h"

using namespace FireCube;
using namespace luabridge;

LuaScript::LuaScript(Engine *engine) : Component(engine), object(engine->GetLuaState()->GetState()), initFunction(engine->GetLuaState()->GetState()), updateFunction(engine->GetLuaState()->GetState())
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
}

void LuaScript::Update(float time)
{	
	if (IsEnabled())
	{
		if (updateFunction.isFunction())
		{
			// Call update function	
			updateFunction(object, this, time);
		}
	}
}

void LuaScript::MarkedDirty()
{

}

void LuaScript::NodeChanged()
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
	
	object = newTable(state);
	object.push(state);
	objectTable.push(state);
	lua_setmetatable(state, -2);
	lua_pop(state, 1);

	object["node"] = GetNode();
	initFunction = objectTable["Init"];
	updateFunction = objectTable["Update"];
	initFunction(object, this);
}