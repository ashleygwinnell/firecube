#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Geometry/CollisionQuery.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Scene/StaticModel.h"
#include "Scene/Light.h"
#include "Scene/ParticleEmitter.h"
#include "Scene/CustomGeometry.h"
#include "Scene/Camera.h"
#include "Scene/AnimatedModel.h"
#include "Physics/CollisionShape.h"
#include "Scripting/LuaScript.h"
#include "Audio/SoundEmitter.h"

using namespace FireCube;
using namespace luabridge;

int GetComponent(lua_State *L)
{
	LuaRef param1 = LuaRef::fromStack(L, -2);
	LuaRef param2 = LuaRef::fromStack(L, -1);
	Node *node = param1.cast<Node *>();
	std::string type = param2;

	Component *component = node->GetComponent(type);

	if (component)
	{
		UserdataPtr::push(L, component, component->GetTypeName().c_str());
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

int CreateComponent(lua_State *L)
{
	int n = lua_gettop(L);
	LuaRef param1 = LuaRef::fromStack(L, 1);
	LuaRef param2 = LuaRef::fromStack(L, 2);
	Node *node = param1.cast<Node *>();
	std::string type = param2;

	if (type == "ParticleEmitter")
	{
		LuaRef param3 = LuaRef::fromStack(L, 3);
		LuaRef param4 = LuaRef::fromStack(L, 4);
		auto component = node->CreateComponent<ParticleEmitter>(param3, param4.cast<Material *>());
		UserdataPtr::push(L, component);
	}
	else if (type == "CustomGeometry")
	{
		auto component = node->CreateComponent<CustomGeometry>();
		UserdataPtr::push(L, component);
	}
	else if (type == "StaticModel")
	{
		StaticModel *component;
		if (lua_gettop(L) == 3)
		{
			LuaRef param3 = LuaRef::fromStack(L, 3);
			component = node->CreateComponent<StaticModel>(param3.cast<Mesh *>());
		}
		else
		{
			component = node->CreateComponent<StaticModel>();
		}

		UserdataPtr::push(L, component);
	}
	else if (type == "CollisionShape")
	{
		auto component = node->CreateComponent<CollisionShape>();
		UserdataPtr::push(L, component);
	}
	else if (type == "LuaScript")
	{
		auto component = node->CreateComponent<LuaScript>();
		UserdataPtr::push(L, component);
	}
	else if (type == "SoundEmitter")
	{
		SoundEmitter *component;
		if (lua_gettop(L) == 3)
		{
			LuaRef param3 = LuaRef::fromStack(L, 3);
			component = node->CreateComponent<SoundEmitter>(param3.cast<Sound *>());
		}
		else
		{
			component = node->CreateComponent<SoundEmitter>();
		}
		
		UserdataPtr::push(L, component);
	}
	else if (type == "AnimatedModel")
	{
		AnimatedModel *component;
		if (lua_gettop(L) == 3)
		{
			LuaRef param3 = LuaRef::fromStack(L, 3);
			component = node->CreateComponent<AnimatedModel>(param3.cast<AnimatedMesh *>());
		}
		else
		{
			component = node->CreateComponent<AnimatedModel>();
		}

		UserdataPtr::push(L, component);
	}
	else if (type == "Light")
	{
		auto component = node->CreateComponent<Light>();
		UserdataPtr::push(L, component);
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

void IntersectRay(Scene *scene, RayQuery *rayQuery, unsigned int collisionQueryMask)
{
	scene->IntersectRay(*rayQuery, collisionQueryMask);
}

Node *NodeNew(Engine *engine, const std::string &name)
{
	return new Node(engine, name);
}

int Clone(lua_State *L)
{
	LuaRef param1 = LuaRef::fromStack(L, -1);	
	Component *component = param1.cast<Component *>();	

	Component *clonedComponent = component->Clone();

	if (clonedComponent)
	{
		UserdataPtr::push(L, clonedComponent, clonedComponent->GetTypeName().c_str());
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

void LuaBindings::InitScene(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Scene>("Scene")
			.addFunction("ClonePrefab", &Scene::ClonePrefab)
			.addFunction("SetPrefab", &Scene::SetPrefab)
			.addFunctionFree("IntersectRay", &IntersectRay)
			.addProperty("camera", &Scene::GetCamera, &Scene::SetCamera)
			.addFunction("GetRootNode", &Scene::GetRootNode)
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
			.addCFunctionFree("CreateComponent", &CreateComponent)
			.addFunction("Remove", &Node::Remove)
			.addProperty("name", &Node::GetName, &Node::SetName)
			.addProperty("children", &Node::GetChildren)
			.addFunction("CreateChild", &Node::CreateChild)
			.addStaticFunction("New", &NodeNew)
			.addFunction("LookAt", &Node::LookAt)
		.endClass()
		.deriveClass<Component, Object>("Component")
			.addProperty("node", &Component::GetNode)
			.addProperty("enabled", &Component::IsEnabled, &Component::SetEnabled)
			.addCFunctionFree("Clone", &Clone)
		.endClass()
		.deriveClass<Renderable, Component>("Renderable")
			.addFunction("SetCollisionQueryMask", &Renderable::SetCollisionQueryMask)
			.addFunction("GetCollisionQueryMask", &Renderable::GetCollisionQueryMask)
			.addProperty("collisionQueryMask", &Renderable::GetCollisionQueryMask, &Renderable::SetCollisionQueryMask)
			.addFunction("SetReceiveShadow", &Renderable::SetReceiveShadow)
			.addFunction("GetReceiveShadow", &Renderable::GetReceiveShadow)
			.addProperty("receiveShadow", &Renderable::GetReceiveShadow, &Renderable::SetReceiveShadow)
			.addFunction("SetCastShadow", &Renderable::SetCastShadow)
			.addFunction("GetCastShadow", &Renderable::GetCastShadow)
			.addProperty("castShadow", &Renderable::GetCastShadow, &Renderable::SetCastShadow)
			.addFunction("SetLightMask", &Renderable::SetLightMask)
			.addFunction("GetLightMask", &Renderable::GetLightMask)
			.addProperty("lightMask", &Renderable::GetLightMask, &Renderable::SetLightMask)
		.endClass()
		.deriveClass<StaticModel, Renderable>("StaticModel")
		.endClass()
		.deriveClass<AnimatedModel, Renderable>("AnimatedModel")
		.endClass()
		.deriveClass<Light, Component>("Light")
			.addProperty("lightType", &Light::GetLightType, &Light::SetLightType)
			.addProperty("castShadow", &Light::GetCastShadow, &Light::SetCastShadow)
			.addProperty("color", &Light::GetColor, &Light::SetColor)
			.addProperty("spotCutOff", &Light::GetSpotCutOff, &Light::SetSpotCutOff)
		.endClass()		
		.deriveClass<ParticleEmitter, Renderable>("ParticleEmitter")
			.addFunction("Reset", &ParticleEmitter::Reset)
			.addFunction("SetMaterial", &ParticleEmitter::SetMaterial)
			.addFunction("GetMaterial", &ParticleEmitter::GetMaterial)
			.addProperty("material", &ParticleEmitter::GetMaterial, &ParticleEmitter::SetMaterial)
		.endClass()
		.deriveClass<CustomGeometry, Renderable>("CustomGeometry")
			.addFunction("AddVertex", &CustomGeometry::AddVertex)
			.addFunction("SetNormal", &CustomGeometry::SetNormal)
			.addFunction("SetTexCoord", &CustomGeometry::SetTexCoord)
			.addFunction("SetMaterial", &CustomGeometry::SetMaterial)
			.addFunction("SetPrimitiveType", &CustomGeometry::SetPrimitiveType)
			.addFunction("UpdateGeometry", &CustomGeometry::UpdateGeometry)
			.addFunction("Clear", &CustomGeometry::Clear)
		.endClass()
		.deriveClass<Camera, Component>("Camera")
			.addFunction("GetPickingRay", &Camera::GetPickingRay)
		.endClass();

	LuaRef t = LuaRef::newTable(luaState);
	t["DIRECTIONAL"] = static_cast<unsigned int>(LightType::DIRECTIONAL);
	t["POINT"] = static_cast<unsigned int>(LightType::POINT);
	t["SPOT"] = static_cast<unsigned int>(LightType::SPOT);
	setGlobal(luaState, t, "LightType");
}