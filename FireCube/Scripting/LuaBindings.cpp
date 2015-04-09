#include "Scripting/LuaBindings.h"
#include "lua.hpp"
#include "LuaBridge.h"
#include "Scene/Node.h"
#include "Math/Math.h"
#include "Scripting/LuaScript.h"
#include "Scene/Scene.h"
#include "Physics/CharacterController.h"
#include "Scene/StaticModel.h"
#include "Scene/Light.h"

using namespace FireCube;
using namespace luabridge;

template <typename T>
T vec_mul(LuaRef a, LuaRef b)
{
	if (a.isUserdata())
	{
		T av = a.cast<T>();
		if (b.isUserdata())
		{
			T bv = b.cast<T>();
			return av * bv;
		}
		else if (b.isNumber())
		{
			return av * ((float)b);
		}
	}
	else if (a.isNumber())
	{
		T bv = b.cast<T>();
		return ((float)a) * bv;
	}

	return T(0.0f);
}

void LuaBindings::Init(lua_State *luaState)
{
	LuaBindings::InitMath(luaState);
	LuaBindings::InitScene(luaState);
	LuaBindings::InitUtils(luaState);
}

void LuaBindings::InitMath(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<vec3>("vec3")
			.addConstructor<void(*) ()>()
			.addConstructor<void(*) (float, float, float)>()
			.addData("x", &vec3::x)
			.addData("y", &vec3::y)
			.addData("z", &vec3::z)
			.addFunction("Length", &vec3::Length)
			.addFunction("Length2", &vec3::Length2)
			.addFunction("Normalized", &vec3::Normalized)
			.addFunctionFree("__add", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec3>)
			.addFunction("__unm", (vec3(vec3::*)()) &vec3::operator-)
			.addStaticFunction("Cross", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::Cross)
		.endClass()
		.beginClass<mat4>("mat4")
			.addFunction("GetDirection", &mat4::GetDirection)
		.endClass();
}

int GetComponent(lua_State *L)
{	
	LuaRef param1 = LuaRef::fromStack(L, -2);
	LuaRef param2 = LuaRef::fromStack(L, -1);
	Node *node = param1.cast<Node *>();
	std::string type = param2;
	if (type == "StaticModel")
	{
		LuaRef ret(L);
		ret = node->GetComponent<StaticModel>();
		ret.push(L);
	}
	else if (type == "Light")
	{
		LuaRef ret(L);
		ret = node->GetComponent<Light>();
		ret.push(L);
	}
	else if (type == "CharacterController")
	{
		LuaRef ret(L);
		ret = node->GetComponent<CharacterController>();
		ret.push(L);
	}
	else
	{
		LuaRef ret(L); // nil
		ret.push(L);
	}
		
	return 1;
}

void LuaBindings::InitScene(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Scene>("Scene")
			.addFunction("ClonePrefab", &Scene::ClonePrefab)
		.endClass()
		.beginClass<Node>("Node")
			.addFunction("Rotate", &Node::Rotate)
			.addFunction("Move", &Node::Move)
			.addFunction("Scale", &Node::Scale)			
			.addProperty("scene", &Node::GetScene)
			.addFunction("GetWorldTransformation", &Node::GetWorldTransformation)
			.addCFunctionFree("GetComponent", &GetComponent)
		.endClass()
		.beginClass<Component>("Component")
			.addProperty("node", &Component::GetNode)
			.addProperty("enabled", &Component::IsEnabled, &Component::SetEnabled)
		.endClass()		
		.deriveClass<Renderable, Component>("Renderable")
		.endClass()
		.deriveClass<StaticModel, Renderable>("StaticModel")
		.endClass()
		.deriveClass<Light, Component>("Light")
		.endClass()
		.deriveClass<CharacterController, Component>("CharacterController")			
			.addProperty("velocity", &CharacterController::GetVelocity, &CharacterController::SetVelocity)
			.addProperty("onGround", &CharacterController::IsOnGround)			
		.endClass()
		.deriveClass <LuaScript, Component>("LuaScript")
			.addFunction("SubscribeToEvent", &LuaScript::SubscribeToEventFromLua)
		.endClass();
}

void LuaBindings::InitUtils(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<StringHash>("StringHash")
			.addConstructor<void(*) ()>()
			.addConstructor<void(*) (const std::string &)>()
		.endClass()
		.beginClass<MappedInput>("MappedInput")
			.addFunction("IsStateOn", &MappedInput::IsStateOn)
			.addFunction("IsActionTriggered", &MappedInput::IsActionTriggered)
			.addFunction("GetValue", &MappedInput::GetValue)
			.addFunction("HasValue", &MappedInput::HasValue)
		.endClass();
}