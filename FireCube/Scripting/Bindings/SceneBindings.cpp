#include "lua.hpp"
#include "sol.hpp"
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
#include "Scene/Prefab.h"
using namespace FireCube;

sol::object GetComponent(Node *node, const std::string &type, sol::this_state s)
{
	if (type == "ParticleEmitter")
	{
		auto component = node->GetComponent<ParticleEmitter>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "CustomGeometry")
	{
		auto component = node->GetComponent<CustomGeometry>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "StaticModel")
	{
		auto component = node->GetComponent<StaticModel>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "CollisionShape")
	{
		auto component = node->GetComponent<CollisionShape>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "LuaScript")
	{
		auto component = node->GetComponent<LuaScript>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "SoundEmitter")
	{
		auto component = node->GetComponent<SoundEmitter>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "AnimatedModel")
	{
		auto component = node->GetComponent<AnimatedModel>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Light")
	{
		auto component = node->GetComponent<Light>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else if (type == "Camera")
	{
		auto component = node->GetComponent<Camera>();
		if (component)
		{
			return sol::object(s, sol::in_place, component);
		}
		else
		{
			return sol::make_object(s, sol::nil);
		}
	}
	else
	{
		return sol::make_object(s, sol::nil);
	}
}

sol::object GetComponents(Node *node, const std::string &type, bool recursive, sol::this_state s)
{
	std::vector<Component *> components;
	node->GetComponents(type, components, recursive);

	return sol::object(s, sol::in_place, components);
}

sol::object GetScriptObject(Node *node, const std::string &objectName, sol::this_state s)
{
	std::vector<LuaScript *> scripts;

	node->GetComponents(scripts);
	
	for (auto &script : scripts)
	{
		if (script->GetObjectName() == objectName)
		{	
			return script->GetScriptObject();
		}
	}	

	return sol::make_object(s, sol::nil);
}

sol::object CreateComponent(Node *node, const std::string &type, sol::variadic_args va, sol::this_state s)
{	
	if (type == "ParticleEmitter")
	{
		sol::object param3 = va[1];
		sol::object param4 = va[2];		
		auto component = node->CreateComponent<ParticleEmitter>(param3.as<unsigned int>(), param4 == sol::nil ? nullptr : param4.as<Material *>());
		return sol::object(s, sol::in_place, component);
	}
	else if (type == "CustomGeometry")
	{
		auto component = node->CreateComponent<CustomGeometry>();
		return sol::object(s, sol::in_place, component);
	}
	else if (type == "StaticModel")
	{
		StaticModel *component;
		if (va.leftover_count() == 1)
		{
			auto param3 = va[0];
			component = node->CreateComponent<StaticModel>(param3.get<Mesh *>());
		}
		else
		{
			component = node->CreateComponent<StaticModel>();
		}

		return sol::object(s, sol::in_place, component);
	}
	else if (type == "CollisionShape")
	{
		auto component = node->CreateComponent<CollisionShape>();
		return sol::object(s, sol::in_place, component);
	}
	else if (type == "LuaScript")
	{
		auto component = node->CreateComponent<LuaScript>();
		return sol::object(s, sol::in_place, component);
	}
	else if (type == "SoundEmitter")
	{
		SoundEmitter *component;
		if (va.leftover_count() == 1)
		{
			auto param3 = va[0];
			component = node->CreateComponent<SoundEmitter>(param3.get<Sound *>());
		}
		else
		{
			component = node->CreateComponent<SoundEmitter>();
		}

		return sol::object(s, sol::in_place, component);
	}
	else if (type == "AnimatedModel")
	{
		AnimatedModel *component;
		if (va.leftover_count() == 1)
		{
			auto param3 = va[0];
			component = node->CreateComponent<AnimatedModel>(param3.get<Mesh *>());
		}
		else
		{
			component = node->CreateComponent<AnimatedModel>();
		}

		return sol::object(s, sol::in_place, component);
	}
	else if (type == "Light")
	{
		auto component = node->CreateComponent<Light>();
		return sol::object(s, sol::in_place, component);
	}
	else if (type == "Camera")
	{
		auto component = node->CreateComponent<Camera>();
		return sol::object(s, sol::in_place, component);
	}
	else
	{
		return sol::make_object(s, sol::nil);
	}	
}

Node *NodeNew(Engine *engine, const std::string &name)
{
	return new Node(engine, name);
}

void LuaBindings::InitScene(sol::state &luaState)
{
	luaState.new_usertype<Scene>("Scene",
		"root", sol::property(&Scene::GetRootNode),
		"IntersectRay", &Scene::IntersectRay,
		"GetRootNode", &Scene::GetRootNode,
		sol::base_classes, sol::bases<Object, RefCounted>());

	luaState.new_usertype<Node>("Node",
		"Rotate", &Node::Rotate,
		"SetRotation", &Node::SetRotation, 
		"rotation", sol::property(&Node::GetRotation, &Node::SetRotation),
		"Move", &Node::Move,
		"SetTranslation", &Node::SetTranslation,
		"translation", sol::property(&Node::GetTranslation, &Node::SetTranslation),
		"Scale", &Node::Scale,
		"SetScale", &Node::SetScale,
		"scale", sol::property(&Node::GetScale, &Node::SetScale),
		"scene", sol::property(&Node::GetScene),
		"GetWorldTransformation", &Node::GetWorldTransformation,
		"GetWorldPosition", &Node::GetWorldPosition,
		"GetComponent", &GetComponent,
		"GetComponents", &GetComponents,
		"CreateComponent", &CreateComponent,
		"RemoveComponent", &Node::RemoveComponent,
		"Remove", &Node::Remove,
		"DelayedRemove", &Node::DelayedRemove,
		"name", sol::property(&Node::GetName, &Node::SetName),
		"children", sol::property([](const Node &self) {
			std::vector<Node *> ret;
			ret.reserve(self.GetChildren().size());
			for (auto &child : self.GetChildren())
			{
				ret.push_back(child.Get());
			}

			return ret;
		}),
		"CreateChild", &Node::CreateChild,
		"new", &NodeNew,
		"LookAt", &Node::LookAt,
		"GetChild", &Node::GetChild,
		"GetScriptObject", &GetScriptObject,
		"Clone", &Node::Clone,
		"parent", sol::property(&Node::GetParent, &Node::SetParent),
		sol::base_classes, sol::bases<Object, RefCounted>());

	luaState.new_usertype<Component>("Component",
		"node", sol::property(&Component::GetNode),
		"enabled", sol::property(&Component::IsEnabled, &Component::SetEnabled),
		"Clone", &Component::Clone,
		sol::base_classes, sol::bases<Object, RefCounted>());

	luaState.new_usertype<RenderablePart>("RenderablePart",
		"material", &RenderablePart::material
	);

	luaState.new_usertype<Renderable>("Renderable",
		"SetCollisionQueryMask", &Renderable::SetCollisionQueryMask,
		"GetCollisionQueryMask", &Renderable::GetCollisionQueryMask,
		"collisionQueryMask", sol::property(&Renderable::GetCollisionQueryMask, &Renderable::SetCollisionQueryMask),
		"SetReceiveShadow", &Renderable::SetReceiveShadow,
		"GetReceiveShadow", &Renderable::GetReceiveShadow,
		"receiveShadow", sol::property(&Renderable::GetReceiveShadow, &Renderable::SetReceiveShadow),
		"SetCastShadow", &Renderable::SetCastShadow,
		"GetCastShadow", &Renderable::GetCastShadow,
		"castShadow", sol::property(&Renderable::GetCastShadow, &Renderable::SetCastShadow),
		"SetLightMask", &Renderable::SetLightMask,
		"GetLightMask", &Renderable::GetLightMask,
		"lightMask", sol::property(&Renderable::GetLightMask, &Renderable::SetLightMask),
		"renderableParts", sol::property([](Renderable &self) {
			std::vector<const RenderablePart *> ret(self.GetRenderableParts().size());
			auto &renderableParts = self.GetRenderableParts();
			std::transform(renderableParts.begin(), renderableParts.end(), ret.begin(), [](const RenderablePart &part) {
				return &part;
			});
			return ret;
		}),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());

	luaState.new_usertype<StaticModel>("StaticModel",
		"GetMaterial", [](StaticModel &self, int index) { return self.GetMaterials()[index].Get(); },
		"materials", sol::property([](StaticModel &self) {
			std::vector<Material *> ret(self.GetMaterials().size());
			auto &materials = self.GetMaterials();
			std::transform(materials.begin(), materials.end(), ret.begin(), [](SharedPtr<Material> &mat) {
				return mat.Get();
			});
			return ret;
		}),
		sol::base_classes, sol::bases<Renderable, Component, Object, RefCounted>());

	luaState.new_usertype<AnimatedModel>("AnimatedModel",
		sol::base_classes, sol::bases<Renderable, Component, Object, RefCounted>());

	luaState.new_usertype<Light>("Light",	
		"GetColor", &Light::GetColor,
		"lightType", sol::property(&Light::GetLightType, &Light::SetLightType),
		"castShadow", sol::property(&Light::GetCastShadow, &Light::SetCastShadow),
		"color", sol::property(&Light::GetColor, &Light::SetColor),
		"spotCutOff", sol::property(&Light::GetSpotCutOff, &Light::SetSpotCutOff),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());

	luaState.new_usertype<ParticleEmitter>("ParticleEmitter",
		"Reset", &ParticleEmitter::Reset,
		"SetBoundingBox", &ParticleEmitter::SetBoundingBox,
		"SetMaterial", &ParticleEmitter::SetMaterial,
		"GetMaterial", &ParticleEmitter::GetMaterial,
		"material", sol::property(&ParticleEmitter::GetMaterial, &ParticleEmitter::SetMaterial),
		"emissionRate", sol::property(&ParticleEmitter::GetEmissionRate, &ParticleEmitter::SetEmissionRate),
		sol::base_classes, sol::bases<Renderable, Component, Object, RefCounted>());

	luaState.new_usertype<CustomGeometry>("CustomGeometry",
		"AddVertex", &CustomGeometry::AddVertex,
		"SetNormal", &CustomGeometry::SetNormal,
		"SetTexCoord", &CustomGeometry::SetTexCoord,
		"SetMaterial", &CustomGeometry::SetMaterial,
		"SetPrimitiveType", &CustomGeometry::SetPrimitiveType,
		"UpdateGeometry", &CustomGeometry::UpdateGeometry,
		"Clear", &CustomGeometry::Clear,
		sol::base_classes, sol::bases<Renderable, Component, Object, RefCounted>());

	luaState.new_usertype<Camera>("Camera",
		"GetPickingRay", &Camera::GetPickingRay,
		sol::base_classes, sol::bases<Component, Object, RefCounted>());

	luaState.new_usertype<SceneReader>("SceneReader",
		sol::constructors<SceneReader(Engine *)>(),
		"Read", &SceneReader::Read);

	luaState.new_usertype<Prefab>("Prefab",
		sol::constructors<Prefab(Engine *)>(),
		"Instantiate", &Prefab::Instantiate,
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	sol::table LightTypeTable = luaState.create_named_table("LightType");	
	LightTypeTable["DIRECTIONAL"] = static_cast<unsigned int>(LightType::DIRECTIONAL);
	LightTypeTable["POINT"] = static_cast<unsigned int>(LightType::POINT);
	LightTypeTable["SPOT"] = static_cast<unsigned int>(LightType::SPOT);	
}