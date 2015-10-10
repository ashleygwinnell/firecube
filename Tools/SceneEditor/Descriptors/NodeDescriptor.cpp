#include "NodeDescriptor.h"
#include "ComponentDescriptor.h"

using namespace FireCube;

NodeDescriptor::NodeDescriptor() : node(nullptr), parent(nullptr), translation(0.0f), scale(1.0f), rotation(0.0f)
{

}

NodeDescriptor::NodeDescriptor(const std::string &name) : NodeDescriptor()
{
	this->name = name;
}

NodeDescriptor::~NodeDescriptor()
{
	for (auto component : components)
	{
		delete component;
	}

	for (auto child : children)
	{
		delete child;
	}
}

Node *NodeDescriptor::GetNode()
{
	return node;
}

void NodeDescriptor::SetNode(Node *node)
{
	this->node = node;
}

void NodeDescriptor::SetParent(NodeDescriptor *parent)
{
	if (this->parent != parent)
	{
		if (this->parent)
		{
			Remove();
		}

		this->parent = parent;
		parent->children.push_back(this);
		node->SetParent(parent->GetNode());
	}
}

NodeDescriptor *NodeDescriptor::GetParent()
{
	return parent;
}

void NodeDescriptor::Remove()
{
	parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
	parent = nullptr;
	node->Remove();
}

void NodeDescriptor::AddComponent(ComponentDescriptor *componentDesc)
{
	components.push_back(componentDesc);	
}

void NodeDescriptor::RemoveComponent(ComponentDescriptor *componentDesc)
{
	components.erase(std::remove(components.begin(), components.end(), componentDesc), components.end());	
}


NodeDescriptor *NodeDescriptor::Clone()
{
	NodeDescriptor *ret = new NodeDescriptor();

	ret->translation = translation;
	ret->scale = scale;
	ret->rotation = rotation;
	ret->name = name;

	for (auto component : components)
	{
		ret->components.push_back(component->Clone());
	}

	for (auto child : children)
	{
		ret->children.push_back(child->Clone());
	}

	return ret;
}

Node *NodeDescriptor::Instantiate(NodeDescriptor *parent, Engine *engine, std::map<FireCube::Node *, NodeDescriptor *> &nodeMap)
{
	node = new Node(engine, name);
	nodeMap[node] = this;
	
	node->SetTranslation(translation);
	node->SetScale(scale);
	mat4 rotationMat = mat4::IDENTITY;
	rotationMat.RotateX(rotation.x);
	rotationMat.RotateY(rotation.y);
	rotationMat.RotateZ(rotation.z);
	node->SetRotation(rotationMat);

	if (parent)
	{
		node->SetParent(parent->GetNode());
	}

	for (auto component : components)
	{
		component->CreateComponent(node, engine);
	}

	for (auto child : children)
	{
		child->Instantiate(this, engine, nodeMap);
	}

	return node;
}

std::vector<NodeDescriptor *> &NodeDescriptor::GetChildren()
{
	return children;
}

std::vector<ComponentDescriptor *> &NodeDescriptor::GetComponents()
{
	return components;
}

void NodeDescriptor::AddChild(NodeDescriptor *child)
{
	children.push_back(child);
}

void NodeDescriptor::RemoveChild(NodeDescriptor *child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void NodeDescriptor::RemoveAllComponents()
{
	for (auto component : components)
	{
		delete component;
	}

	components.clear();
}

void NodeDescriptor::SetTranslation(vec3 translation)
{
	this->translation = translation;
	if (node)
	{
		node->SetTranslation(translation);
	}
}

vec3 NodeDescriptor::GetTranslation() const
{
	return translation;
}

void NodeDescriptor::SetScale(vec3 scale)
{
	this->scale = scale;
	if (node)
	{
		node->SetScale(scale);
	}
}

vec3 NodeDescriptor::GetScale() const
{
	return scale;
}

void NodeDescriptor::SetRotation(vec3 rotation)
{
	this->rotation = rotation;
	if (node)
	{
		mat4 rotationMat = mat4::IDENTITY;
		rotationMat.RotateX(rotation.x);
		rotationMat.RotateY(rotation.y);
		rotationMat.RotateZ(rotation.z);

		node->SetRotation(rotationMat);
	}
}

vec3 NodeDescriptor::GetRotation() const
{
	return rotation;
}

void NodeDescriptor::SetName(const std::string &name)
{
	this->name = name;
	if (node)
	{
		node->SetName(name);
	}
}

std::string NodeDescriptor::GetName() const
{
	return name;
}