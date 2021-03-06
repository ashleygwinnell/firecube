#include "NodeDescriptor.h"
#include "ComponentDescriptor.h"

using namespace FireCube;

NodeDescriptor::NodeDescriptor() : node(nullptr), parent(nullptr), translation(0.0f), scale(1.0f), rotation(0.0f), isPrefab(false)
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

	if (node.Expired() == false)
	{
		node->RemoveImmediately();
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
		SharedPtr<Node> nodePtr = node.Lock(); // Keep the node alive so the Remove call below won't delete the node
		if (this->parent)
		{
			Remove();
		}

		this->parent = parent;
		parent->children.push_back(this);
		if (node)
		{
			node->SetParent(parent->GetNode());
		}
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
	node->RemoveImmediately();
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
	ret->isPrefab = isPrefab;
	ret->prefabPath = prefabPath;

	for (auto component : components)
	{
		auto clonedComponent = component->Clone();
		clonedComponent->SetParent(ret);
		ret->components.push_back(clonedComponent);
	}

	for (auto child : children)
	{
		auto clone = child->Clone();
		clone->SetParent(ret);		
	}

	return ret;
}

Node *NodeDescriptor::Instantiate(NodeDescriptor *parent, Engine *engine, std::map<FireCube::Node *, NodeDescriptor *> &nodeMap)
{
	node = new Node(engine, name);
	nodeMap[node] = this;
	
	node->SetTranslation(translation);
	node->SetScale(scale);
	quat rot(rotation.x, rotation.y, rotation.z);	
	node->SetRotation(rot);

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

void NodeDescriptor::RemoveAllChildren()
{
	for (auto child : children)
	{
		delete child;
	}

	children.clear();
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
		node->SetRotation(quat(rotation.x, rotation.y, rotation.z));
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

bool NodeDescriptor::IsPrefab() const
{
	return isPrefab;
}

void NodeDescriptor::SetIsPrefab(bool val)
{
	isPrefab = val;
}

std::string NodeDescriptor::GetPrefabPath() const
{
	return prefabPath;
}

void NodeDescriptor::SetPrefabPath(std::string val)
{
	prefabPath = val;
}

void NodeDescriptor::SetChildIndex(NodeDescriptor *child, unsigned int index)
{
	auto oldIter = std::find(children.begin(), children.end(), child);
	if (oldIter != children.end())
	{
		auto oldIndex = std::distance(children.begin(), oldIter);
		if (oldIndex != index)
		{			
			children.erase(oldIter);
			children.insert(children.begin() + index, child);
		}
	}
}