#include "ComponentDescriptor.h"
#include "NodeDescriptor.h"

std::map<ComponentType, std::string> ComponentDescriptor::componentTypeToName = {
	{ ComponentType::CHARACTER_CONTROLLER, "CharacterController" },
	{ ComponentType::COLLISION_SHAPE, "CollisionShape" },
	{ ComponentType::STATIC_MODEL, "StaticModel" },
	{ ComponentType::PHYSICS_WORLD, "PhysicsWorld" },
	{ ComponentType::LUA_SCRIPT, "LuaScript" },
	{ ComponentType::LIGHT, "Light" },
	{ ComponentType::BOX, "Box" }
};

ComponentDescriptor::ComponentDescriptor(ComponentType type) : type(type), component(nullptr), parent(nullptr)
{

}
void ComponentDescriptor::Remove(NodeDescriptor *parent)
{	
	parent->RemoveComponent(this);
	component->GetNode()->RemoveComponent(component);
	component = nullptr;
}

FireCube::Component *ComponentDescriptor::GetComponent()
{
	return component;
}

void ComponentDescriptor::SetParent(NodeDescriptor *nodeDesc)
{
	this->parent = nodeDesc;
}

NodeDescriptor *ComponentDescriptor::GetParent()
{
	return parent;
}

ComponentType ComponentDescriptor::GetType() const
{
	return type;
}

std::string ComponentDescriptor::GetTypeName() const
{
	auto i = componentTypeToName.find(type);
	if (i != componentTypeToName.end())
	{
		return i->second;
	}
	else
	{
		return "";
	}
}