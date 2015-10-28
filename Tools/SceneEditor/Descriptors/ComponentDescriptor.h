#pragma once

#include "FireCube.h"

class NodeDescriptor;

enum class ComponentType
{
	STATIC_MODEL, LIGHT, LUA_SCRIPT, COLLISION_SHAPE, CHARACTER_CONTROLLER, PHYSICS_WORLD, BOX
};

class ComponentDescriptor
{
public:
	ComponentDescriptor(ComponentType type);
	virtual void CreateComponent(FireCube::Node *node, FireCube::Engine *engine) = 0;
	virtual ComponentDescriptor *Clone() = 0;
	virtual ~ComponentDescriptor() {};
	void Remove(NodeDescriptor *parent);
	FireCube::Component *GetComponent();
	void SetParent(NodeDescriptor *nodeDesc);
	NodeDescriptor *GetParent();
	ComponentType GetType() const;
	std::string GetTypeName() const;
protected:
	FireCube::Component *component;
	NodeDescriptor *parent;
	ComponentType type;

private:
	static std::map<ComponentType, std::string> componentTypeToName;
	
};