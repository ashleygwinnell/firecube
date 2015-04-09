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
#include "Scene/Camera.h"
#include "Core/Engine.h"
#include "Rendering/Renderer.h"

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

template <typename T>
struct MatHelper
{
	template <unsigned index>
	static float Get(T const* mat)
	{
		return mat->m[index];
	}

	template <unsigned index>
	static void Set(T* mat, float value)
	{
		mat->m[index] = value;
	}
};

void LuaBindings::Init(lua_State *luaState, Engine *engine)
{
	LuaBindings::InitMath(luaState);
	LuaBindings::InitScene(luaState);
	LuaBindings::InitUtils(luaState);
	LuaBindings::InitRenderer(luaState);
	LuaBindings::InitEngine(luaState, engine);
}

void LuaBindings::InitMath(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<vec3>("vec3")			
			.addConstructor<void(*) (float, float, float)>()
			.addData("x", &vec3::x)
			.addData("y", &vec3::y)
			.addData("z", &vec3::z)
			.addFunction("Length", &vec3::Length)
			.addFunction("Length2", &vec3::Length2)
			.addFunction("Normalized", &vec3::Normalized)
			.addFunction("Dot", &vec3::Dot)
			.addFunctionFree("__add", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec3>)
			.addFunction("__unm", (vec3(vec3::*)()) &vec3::operator-)
			.addStaticFunction("Cross", (vec3(*)(const vec3 &, const vec3 &)) &FireCube::Cross)			
		.endClass()
		.beginClass<vec2>("vec2")			
			.addConstructor<void(*) (float, float)>()
			.addData("x", &vec2::x)
			.addData("y", &vec2::y)		
			.addFunction("Length", &vec2::Length)
			.addFunction("Length2", &vec2::Length2)			
			.addFunctionFree("__add", (vec2(*)(const vec2 &, const vec2 &)) &FireCube::operator+)
			.addFunctionFree("__sub", (vec2(*)(const vec2 &, const vec2 &)) &FireCube::operator-)
			.addFunctionFree("__mul", &vec_mul<vec2>)
			.addFunction("__unm", (vec2(vec2::*)()) &vec2::operator-)
			.addStaticFunction("Cross", (float(*)(const vec2 &, const vec2 &)) &FireCube::Cross)
		.endClass()
		.beginClass<Ray>("Ray")
			.addData("origin", &Ray::origin)
			.addData("direction", &Ray::direction)
		.endClass()
		.beginClass<Plane>("Plane")
			.addConstructor<void(*) (const vec3 &, float)>()
			.addProperty("normal", &Plane::GetNormal)
			.addProperty("distance", &Plane::GetDistance)			
		.endClass()
		.beginClass<mat4>("mat4")
			.addConstructor<void(*) ()>()			
			.addFunction("RotateX", &mat4::RotateX)
			.addFunction("RotateY", &mat4::RotateY)
			.addFunction("RotateZ", &mat4::RotateZ)
			.addFunction("GetDirection", &mat4::GetDirection)
			.addFunction("Identity", &mat4::Identity)			
			.addFunction("Translate", (void(mat4::*)(const vec3 &)) &mat4::Translate)
			.addProperty("m00", &MatHelper<mat4>::Get<0>, &MatHelper<mat4>::Set<0>)
			.addProperty("m10", &MatHelper<mat4>::Get<1>, &MatHelper<mat4>::Set<1>)
			.addProperty("m20", &MatHelper<mat4>::Get<2>, &MatHelper<mat4>::Set<2>)
			.addProperty("m30", &MatHelper<mat4>::Get<3>, &MatHelper<mat4>::Set<3>)
			.addProperty("m01", &MatHelper<mat4>::Get<4>, &MatHelper<mat4>::Set<4>)
			.addProperty("m11", &MatHelper<mat4>::Get<5>, &MatHelper<mat4>::Set<5>)
			.addProperty("m21", &MatHelper<mat4>::Get<6>, &MatHelper<mat4>::Set<6>)
			.addProperty("m31", &MatHelper<mat4>::Get<7>, &MatHelper<mat4>::Set<7>)
			.addProperty("m02", &MatHelper<mat4>::Get<8>, &MatHelper<mat4>::Set<8>)
			.addProperty("m12", &MatHelper<mat4>::Get<9>, &MatHelper<mat4>::Set<9>)
			.addProperty("m22", &MatHelper<mat4>::Get<10>, &MatHelper<mat4>::Set<10>)
			.addProperty("m32", &MatHelper<mat4>::Get<11>, &MatHelper<mat4>::Set<11>)
			.addProperty("m03", &MatHelper<mat4>::Get<12>, &MatHelper<mat4>::Set<12>)
			.addProperty("m13", &MatHelper<mat4>::Get<13>, &MatHelper<mat4>::Set<13>)
			.addProperty("m23", &MatHelper<mat4>::Get<14>, &MatHelper<mat4>::Set<14>)
			.addProperty("m33", &MatHelper<mat4>::Get<15>, &MatHelper<mat4>::Set<15>)
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
	else if (type == "Camera")
	{
		LuaRef ret(L);
		ret = node->GetComponent<Camera>();
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
			.addProperty("camera", &Scene::GetCamera, &Scene::SetCamera)
		.endClass()
		.beginClass<Node>("Node")
			.addFunction("Rotate", &Node::Rotate)
			.addFunction("SetRotation", &Node::SetRotation)
			.addProperty("rotation", &Node::GetRotation, &Node::SetRotation)
			.addFunction("Move", &Node::Move)
			.addFunction("SetTranslation", &Node::SetTranslation)
			.addProperty("translation", &Node::GetTranslation, &Node::SetTranslation)
			.addFunction("Scale", &Node::Scale)
			.addFunction("SetScale", &Node::SetScale)
			.addProperty("scale", &Node::GetScale, &Node::SetScale)
			.addProperty("scene", &Node::GetScene)
			.addFunction("GetWorldTransformation", &Node::GetWorldTransformation)
			.addFunction("GetWorldPosition", &Node::GetWorldPosition)			
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
		.deriveClass<Camera, Component>("Camera")
			.addFunction("GetPickingRay", &Camera::GetPickingRay)
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
			.addConstructor<void(*) (const std::string &)>()
		.endClass()
		.beginClass<MappedInput>("MappedInput")
			.addFunction("IsStateOn", &MappedInput::IsStateOn)
			.addFunction("IsActionTriggered", &MappedInput::IsActionTriggered)
			.addFunction("GetValue", &MappedInput::GetValue)
			.addFunction("HasValue", &MappedInput::HasValue)
		.endClass();
}

void LuaBindings::InitRenderer(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Renderer>("Renderer")
			.addProperty("width", &Renderer::GetWidth)
			.addProperty("height", &Renderer::GetHeight)
		.endClass();
}

void LuaBindings::InitEngine(lua_State *luaState, Engine *engine)
{
	getGlobalNamespace(luaState)
		.beginClass<Engine>("Engine")
			.addProperty("renderer", &Engine::GetRenderer)
		.endClass();

	luabridge::setGlobal(luaState, engine, "engine");
}