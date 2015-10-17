#pragma once

#include "FireCube.h"

class ComponentDescriptor;

class NodeDescriptor
{
public:
	
	NodeDescriptor();
	NodeDescriptor(const std::string &name);
	~NodeDescriptor();
	FireCube::Node *GetNode();
	void SetNode(FireCube::Node *node);
	void SetParent(NodeDescriptor *parent);
	NodeDescriptor *GetParent();
	void Remove();
	void AddComponent(ComponentDescriptor *componentDesc);
	void RemoveComponent(ComponentDescriptor *componentDesc);
	NodeDescriptor *Clone();
	FireCube::Node *Instantiate(NodeDescriptor *parent, FireCube::Engine *engine, std::map<FireCube::Node *, NodeDescriptor *> &nodeMap);
	std::vector<NodeDescriptor *> &GetChildren();
	std::vector<ComponentDescriptor *> &GetComponents();
	void AddChild(NodeDescriptor *child);
	void RemoveChild(NodeDescriptor *child);
	void RemoveAllComponents();
	void RemoveAllChildren();

	void SetTranslation(FireCube::vec3 translation);
	FireCube::vec3 GetTranslation() const;
	void SetScale(FireCube::vec3 scale);
	FireCube::vec3 GetScale() const;
	void SetRotation(FireCube::vec3 rotation);
	FireCube::vec3 GetRotation() const;
	void SetName(const std::string &name);
	std::string GetName() const;
private:
	FireCube::WeakPtr<FireCube::Node> node;
	NodeDescriptor *parent;
	std::vector<ComponentDescriptor *> components;
	std::vector<NodeDescriptor *> children;

	FireCube::vec3 translation;
	FireCube::vec3 rotation;
	FireCube::vec3 scale;

	std::string name;
};