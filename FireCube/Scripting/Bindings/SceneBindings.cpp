#include "lua.hpp"
#include "LuaIntf.h"
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
#include "Scene/SceneReader.h"

using namespace FireCube;
using namespace LuaIntf;

int GetComponent(Node *node, lua_State *L, const std::string &type)
{
	Component *component = node->GetComponent(type);

	if (component)
	{
		Lua::push(L, component);
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

int GetComponents(Node *node, lua_State *L, const std::string &type, bool recursive)
{
	std::vector<Component *> components;
	node->GetComponents(type, components, recursive);

	Lua::push(L, components);

	return 1;
}

int GetScriptObject(Node *node, lua_State *L, const std::string &objectName)
{
	std::vector<LuaScript *> scripts;

	node->GetComponents(scripts, true);
	bool found = false;

	for (auto &script : scripts)
	{
		if (script->GetObjectName() == objectName)
		{
			script->PushObject();
			found = true;
			break;
		}
	}
	
	if (!found)
	{
		lua_pushnil(L);
	}

	return 1;
}

int CreateComponent(Node *node, lua_State *L, const std::string &type)
{
	int n = lua_gettop(L);		
	
	if (type == "ParticleEmitter")
	{
		LuaRef param4 = Lua::pop(L);
		LuaRef param3 = Lua::pop(L);
		
		auto component = node->CreateComponent<ParticleEmitter>(param3.toValue<unsigned int>(), param4.type() == LuaTypeID::NIL ? nullptr : param4.toValue<Material *>());
		Lua::push(L, component);
	}
	else if (type == "CustomGeometry")
	{
		auto component = node->CreateComponent<CustomGeometry>();
		Lua::push(L, component);
	}
	else if (type == "StaticModel")
	{
		StaticModel *component;
		if (n == 3)
		{
			LuaRef param3 = Lua::pop(L);
			component = node->CreateComponent<StaticModel>(param3.toValue<Mesh *>());
		}
		else
		{
			component = node->CreateComponent<StaticModel>();
		}

		Lua::push(L, component);
	}
	else if (type == "CollisionShape")
	{
		auto component = node->CreateComponent<CollisionShape>();
		Lua::push(L, component);
	}
	else if (type == "LuaScript")
	{
		auto component = node->CreateComponent<LuaScript>();
		Lua::push(L, component);
	}
	else if (type == "SoundEmitter")
	{
		SoundEmitter *component;
		if (n == 3)
		{
			LuaRef param3 = Lua::pop(L);
			component = node->CreateComponent<SoundEmitter>(param3.toValue<Sound *>());
		}
		else
		{
			component = node->CreateComponent<SoundEmitter>();
		}
		
		Lua::push(L, component);
	}
	else if (type == "AnimatedModel")
	{
		AnimatedModel *component;
		if (n == 3)
		{
			LuaRef param3 = Lua::pop(L);
			component = node->CreateComponent<AnimatedModel>(param3.toValue<Mesh *>());
		}
		else
		{
			component = node->CreateComponent<AnimatedModel>();
		}

		Lua::push(L, component);
	}
	else if (type == "Light")
	{
		auto component = node->CreateComponent<Light>();
		Lua::push(L, component);
	}
	else
	{
		lua_pushnil(L);
	}

	return 1;
}

Node *NodeNew(Engine *engine, const std::string &name)
{
	return new Node(engine, name);
}

void LuaBindings::InitScene(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginClass<Scene>("Scene")
			.addFunction("ClonePrefab", &Scene::ClonePrefab)
			.addFunction("SetPrefab", &Scene::SetPrefab)
			.addFunction("IntersectRay", &Scene::IntersectRay)
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
			.addFunction("GetComponent", &GetComponent)
			.addFunction("GetComponents", &GetComponents)
			.addFunction("CreateComponent", &CreateComponent)
			.addFunction("Remove", &Node::Remove)
			.addProperty("name", &Node::GetName, &Node::SetName)
			.addProperty("children", &Node::GetChildren)
			.addFunction("CreateChild", &Node::CreateChild)
			.addStaticFunction("New", &NodeNew)
			.addFunction("LookAt", &Node::LookAt)
			.addFunction("GetChild", &Node::GetChild)
			.addFunction("GetScriptObject", &GetScriptObject)
		.endClass()
		.beginExtendClass<Component, Object>("Component")
			.addProperty("node", &Component::GetNode)
			.addProperty("enabled", &Component::IsEnabled, &Component::SetEnabled)
			.addFunction("Clone", &Component::Clone)
		.endClass()
		.beginExtendClass<Renderable, Component>("Renderable")
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
		.beginExtendClass<StaticModel, Renderable>("StaticModel")
		.endClass()
		.beginExtendClass<AnimatedModel, Renderable>("AnimatedModel")
		.endClass()
		.beginExtendClass<Light, Component>("Light")
			.addProperty("lightType", &Light::GetLightType, &Light::SetLightType)
			.addProperty("castShadow", &Light::GetCastShadow, &Light::SetCastShadow)
			.addProperty("color", &Light::GetColor, &Light::SetColor)
			.addProperty("spotCutOff", &Light::GetSpotCutOff, &Light::SetSpotCutOff)
		.endClass()		
		.beginExtendClass<ParticleEmitter, Renderable>("ParticleEmitter")
			.addFunction("Reset", &ParticleEmitter::Reset)
			.addFunction("SetBoundingBox", &ParticleEmitter::SetBoundingBox)
			.addFunction("SetMaterial", &ParticleEmitter::SetMaterial)
			.addFunction("GetMaterial", &ParticleEmitter::GetMaterial)
			.addProperty("material", &ParticleEmitter::GetMaterial, &ParticleEmitter::SetMaterial)
		.endClass()
		.beginExtendClass<CustomGeometry, Renderable>("CustomGeometry")
			.addFunction("AddVertex", &CustomGeometry::AddVertex)
			.addFunction("SetNormal", &CustomGeometry::SetNormal)
			.addFunction("SetTexCoord", &CustomGeometry::SetTexCoord)
			.addFunction("SetMaterial", &CustomGeometry::SetMaterial)
			.addFunction("SetPrimitiveType", &CustomGeometry::SetPrimitiveType)
			.addFunction("UpdateGeometry", &CustomGeometry::UpdateGeometry)
			.addFunction("Clear", &CustomGeometry::Clear)
		.endClass()
		.beginExtendClass<Camera, Component>("Camera")
			.addFunction("GetPickingRay", &Camera::GetPickingRay)
		.endClass()
		.beginClass<SceneReader>("SceneReader")
			.addConstructor(LUA_ARGS(Engine *))
			.addFunction("Read", &SceneReader::Read)
		.endClass();

	LuaRef t = LuaRef::createTable(luaState);
	t["DIRECTIONAL"] = static_cast<unsigned int>(LightType::DIRECTIONAL);
	t["POINT"] = static_cast<unsigned int>(LightType::POINT);
	t["SPOT"] = static_cast<unsigned int>(LightType::SPOT);
	Lua::setGlobal(luaState, "LightType", t);
}