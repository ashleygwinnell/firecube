#include "Utils/Logger.h"
#include "Scripting/LuaScript.h"
#include "Scripting/LuaFunction.h"
#include "Core/Events.h"
#include "Core/Engine.h"
#include "Scripting/LuaState.h"
#include "Core/Variant.h"
#include "Physics/CollisionShape.h"
#include "Physics/CharacterController.h"
#include "Scene/Node.h"

using namespace FireCube;

LuaScript::LuaScript(Engine *engine) : Component(engine), luaFile(nullptr), awakeCalled(false)
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
}

LuaScript::LuaScript(const LuaScript &other) : Component(other), objectName(other.objectName), luaFile(other.luaFile), initialProperties(other.initialProperties), awakeCalled(false)
{
	SubscribeToEvent(Events::Update, &LuaScript::Update);
	CreateObject(objectName);
}

LuaScript::~LuaScript()
{
	CallMemberFunction("Destroy");
}

void LuaScript::Update(float dt)
{	
	if (awakeCalled == false && scene)
	{		
		awakeCalled = true;

		auto awakeFunction = scriptFunctions[ScriptFunction::AWAKE];
		if (objectName.empty() == false && IsEnabled() && awakeFunction)
		{				
			// Call awake function	
			(*awakeFunction)(object);			
		}
	}

	auto updateFunction = scriptFunctions[ScriptFunction::UPDATE];
	if (awakeCalled && IsEnabled() && updateFunction)
	{
		// Call update function	
		(*updateFunction)(object, dt);			
	}
}

void LuaScript::MarkedDirty()
{

}

void LuaScript::NodeChanged()
{	
	if (object.valid())
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
	
}

void LuaScript::CreateObject(const std::string &objectName)
{	
	sol::state &state = engine->GetLuaState()->GetState();
	sol::object objectTable = state[objectName];
	if (objectTable.is<sol::table>() == false)
	{
		LOGERROR("Can't init script object: ", objectName, " is not a table");
		return;
	}
		
	this->objectName = objectName;	
	object = state.create_table();		
	sol::table metatable = state.create_table();
	metatable["__index"] = objectTable;		
	object.set(sol::metatable_key, metatable);
	
	object["script"] = this;
	scriptFunctions[ScriptFunction::INIT] = GetMemberFunction("Init");	
	scriptFunctions[ScriptFunction::UPDATE] = GetMemberFunction("Update");	
	scriptFunctions[ScriptFunction::AWAKE] = GetMemberFunction("Awake");
	
	SetInitialProperties();

	auto initFunction = scriptFunctions[ScriptFunction::INIT];
	if (node && initFunction)
	{
		object["node"] = node;
		(*initFunction)(object);		
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

void LuaScript::SubscribeToEventFromLua(const std::string &eventName, sol::object param)
{
	Object *sender = nullptr;	
	if (param.is<sol::function>() == false)
	{		
		sender = param.as<Object *>();
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

void LuaScript::HandleInput(float dt, const MappedInput &input)
{
	if (awakeCalled)
	{
		(*scriptFunctions[ScriptFunction::HANDLE_INPUT])(object, dt, input);		
	}
}

void LuaScript::CharacterControllerCollision(CharacterController *characterController, CollisionShape *collisionShape)
{
	if (awakeCalled)
	{
		(*scriptFunctions[ScriptFunction::CHARACTER_CONTROLLER_COLLISION])(object, characterController, collisionShape);		
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

void LuaScript::SetInitialPropertyValue(const std::string &property, const std::string &value)
{
	initialProperties[property] = value;
}

void LuaScript::SetInitialProperties()
{
	for (auto &p : initialProperties)
	{
		auto &propertyName = p.first;
		auto &propertyValue = p.second;
		Variant v = Variant::FromString(propertyValue);

		switch (v.GetType())
		{
		case VariantType::BOOL:
			SetProperty(propertyName, v.GetBool());
			break;
		case VariantType::FLOAT:
			SetProperty(propertyName, v.GetFloat());
			break;
		case VariantType::INT:
			SetProperty(propertyName, v.GetInt());
			break;
		case VariantType::VEC2:
			SetProperty(propertyName, v.GetVec2());
			break;
		case VariantType::VEC3:
			SetProperty(propertyName, v.GetVec3());
			break;
		case VariantType::VEC4:
			SetProperty(propertyName, v.GetVec4());
			break;
		case VariantType::NONE:
			SetProperty(propertyName, propertyValue);
			break;
		default:
			break;
		}
	}
}

void LuaScript::PushObject()
{	
	object.push();	
}

sol::table FireCube::LuaScript::GetScriptObject()
{
	return object;
}
