#include "Scripting/LuaBindings.h"
#include "lua.hpp"
#include "LuaBridge.h"
#include "Scene/Node.h"
#include "Math/Math.h"
#include "Scripting/LuaScript.h"
#include "Scene/Scene.h"

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

void LuaBindings::InitMath(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<vec3>("vec3")
			.addConstructor<void(*) ()>()
			.addConstructor<void(*) (float, float, float)>()
			.addData("x", &vec3::x)
			.addData("y", &vec3::y)
			.addData("z", &vec3::z)
			.addFunctionFree("__add", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec3>)
			.addFunction("__unm", (vec3(vec3::*)()) &vec3::operator-)
		.endClass();
}

Component *GetComponent(Node *node, const std::string &type)
{
	return node->GetComponent(type);
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
			.addFunction("GetScene", &Node::GetScene)
			.addProperty("scene", &Node::GetScene)
			.addFunctionFree("GetComponent", &GetComponent)				
		.endClass()
		.beginClass<Component>("Component")
			.addFunction("GetNode", &Component::GetNode)
			.addProperty("enabled", &Component::IsEnabled, &Component::SetEnabled)
		.endClass()
		.deriveClass <LuaScript, Component>("LuaScript")
			.addFunction("SubscribeToEvent", &LuaScript::SubscribeToEventFromLua)
		.endClass()
		.beginClass<MappedInput>("MappedInput")
			.addFunction("IsStateOn", &MappedInput::IsStateOn)
			.addFunction("IsActionTriggered", &MappedInput::IsActionTriggered)
			.addFunction("GetValue", &MappedInput::GetValue)
			.addFunction("HasValue", &MappedInput::HasValue)
		.endClass();
}

void LuaBindings::InitUtils(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<StringHash>("StringHash")
			.addConstructor<void(*) ()>()
			.addConstructor<void(*) (const std::string &)>()
		.endClass();
}