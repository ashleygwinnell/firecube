#include <sstream>
#include "Scene/Node.h"
#include "Utils/Logger.h"

#include "Utils/Filesystem.h"
#include "Core/Engine.h"
#include "Scene/Scene.h"

using namespace FireCube;

Node::Node(Engine *engine) : Object(engine), parent(nullptr), scene(nullptr)
{
	rotation.Identity(); // Can't use mat4::IDENTITY since it might not be initialized yet.
	worldRotation.Identity();
	translation.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	worldTransformation.Identity();
	transformationChanged = false;	
}

Node::Node(Engine *engine, const std::string &name) : Node(engine)
{
	SetName(name);
}

Node::~Node()
{
	RemoveAllComponents();	
}

void Node::SetName(const std::string &name)
{
	this->name = name;
}

std::string Node::GetName() const
{
	return name;
}

Node *Node::GetParent()
{
	return parent;
}

mat4 Node::GetLocalTransformation()
{	
	localTransformation.Identity();
	localTransformation.Translate(translation);
	localTransformation *= rotation;
	localTransformation.Scale(scale.x, scale.y, scale.z);
	

	return localTransformation;
}

mat4 Node::GetWorldTransformation()
{
	if (transformationChanged)
	{
		UpdateWorldTransformation();
	}
	
	return worldTransformation;	
}

mat4 Node::GetWorldRotation()
{
	if (transformationChanged)
	{
		UpdateWorldTransformation();
	}

	return worldRotation;
}

void Node::UpdateWorldTransformation()
{
	transformationChanged = false;
	if (parent)
	{
		worldTransformation = parent->GetWorldTransformation() * GetLocalTransformation();
		worldRotation = parent->GetWorldRotation() * rotation;
	}
	else
	{
		worldTransformation = GetLocalTransformation();
		worldRotation = rotation;
	}
}

void Node::SetTranslation(const vec3 &t)
{
	translation = t;
	SetTransformationChanged();
}

vec3 Node::GetTranslation() const
{
	return translation;
}

void Node::SetRotation(const mat4 &r)
{
	rotation = r;
	SetTransformationChanged();
}

mat4 Node::GetRotation() const
{
	return rotation;
}

void Node::SetScale(const vec3 &s)
{
	scale = s;
	SetTransformationChanged();
}

vec3 Node::GetScale() const
{
	return scale;
}

void Node::Move(const vec3 &t)
{
	translation += t;
	SetTransformationChanged();
}

void Node::Rotate(const vec3 &r)
{
	rotation.RotateX(r.x);
	rotation.RotateY(r.y);
	rotation.RotateZ(r.z);
	SetTransformationChanged();
}

void Node::Scale(const vec3 &s)
{
	scale.x *= s.x;
	scale.y *= s.y;
	scale.z *= s.z;
	SetTransformationChanged();
}

void Node::LookAt(vec3 position, vec3 at, vec3 up)
{
	this->translation = position;
	mat4 temp;
	temp.LookAt(vec3(0, 0, 0), at - position, up);
	rotation = temp;
	rotation.Transpose();
	SetTransformationChanged();
}

Node *Node::AddChild(Node *node)
{
	SharedPtr<Node> sharedNode(node);
	children.push_back(node);
	// If the node has a parent, remove it from the parent's children list
	if (node->parent)
		node->parent->RemoveChild(node);
	node->parent = this;
	
	Scene *oldScene = node->scene;
	node->scene = scene;	
	if (oldScene != scene)
	{
		node->SceneChanged(oldScene);
	}

	node->SetTransformationChanged();
	return node;
}

Node *Node::CreateChild(const std::string &name)
{
	Node *child = new Node(engine, name);
	AddChild(child);
	child->transformationChanged = transformationChanged;
	return child;
}

void Node::SetParent(Node *parent)
{	
	if (parent == nullptr)
	{		
		SharedPtr<Node> sharedNode(this);
		if (this->parent)
		{
			this->parent->RemoveChild(this);
		}

		this->parent = nullptr;
		Scene *oldScene = scene;
		scene = nullptr;
		if (oldScene != scene)
		{
			SceneChanged(oldScene);
		}
	}
	else
	{
		parent->AddChild(this);
	}
	
}

Node *Node::GetChild(const std::string &name, bool recursive)
{
	for (auto i : children)
	{
		if (i->GetName() == name)
			return i;
		if (recursive)
		{
			Node *node = i->GetChild(name, true);
			if (node)
				return node;
		}
	}
	
	return nullptr;
}

void Node::RemoveChild(Node *node)
{
	for (auto i = children.begin(); i != children.end(); i++)
	{
		if (i->Get() == node)
		{			
			children.erase(i);			
			break;
		}
	}		
}

void Node::RemoveChild(const std::string &name)
{
	for (auto i = children.begin(); i != children.end(); i++)
	{
		if ((*i)->GetName() == name)
		{		
			children.erase(i);			
			break;
		}
	}		
}

void Node::RemoveAllChildren()
{
	children.clear();
}

std::vector<SharedPtr<Node>> Node::GetChildren()
{
	return children;
}


Node *Node::Clone() const
{
	Node *ret = new Node(engine);
	//TODO: Implement
	ret->name = name;
	ret->translation = translation;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->localTransformation = localTransformation;
	ret->transformationChanged = transformationChanged;
	ret->worldTransformation = worldTransformation;	
	ret->worldRotation = worldRotation;
	for (auto i = children.begin(); i != children.end(); i++)
	{
		Node *c = (*i)->Clone();
		c->SetParent(ret);
	}
	return ret;
}

void Node::SetTransformationChanged()
{
	if (transformationChanged)
		return;
	
	transformationChanged = true;
	for (auto &component : components)
	{
		component->MarkedDirty();
	}
	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->SetTransformationChanged();
	}
}

vec3 Node::GetWorldPosition()
{
	return GetWorldTransformation() * vec3(0.0f, 0.0f, 0.0f);
}

void Node::AddComponent(Component *component)
{
	if (component->node != nullptr)
	{
		LOGERROR("Attempt to add a component which already has a parent node.");
	}
	else
	{
		components.push_back(component);
		component->SetNode(this);
		component->SetScene(scene);
	}
}

void Node::RemoveComponent(Component *component)
{
	for (auto i = components.begin(); i != components.end(); ++i)
	{
		if (*i == component)
		{
			component->SetNode(nullptr);
			components.erase(i);
			delete component;
			return;
		}		
	}
}

void Node::SetScene(Scene *scene)
{
	this->scene = scene;
}

Scene *Node::GetScene()
{
	return scene;
}

void Node::RemoveAllComponents()
{
	for (auto component : components)
	{
		component->SetNode(nullptr);
		delete component;
	}

	components.clear();
}

Component *Node::GetComponent(const StringHash &type)
{
	for (auto c : components)
	{
		if (c->GetType() == type)
		{
			return c;
		}
	}

	return nullptr;
}

Node *Node::GetRootNode() const
{
	if (scene)
	{
		return scene->GetRootNode();
	}
	
	return nullptr;
}

std::vector<Component *> &Node::GetComponents()
{
	return components;
}

void Node::SceneChanged(Scene *oldScene)
{
	for (auto component : components)
	{
		component->SetScene(scene);
	}

	for (auto child : children)
	{
		child->SceneChanged(oldScene);
	}
}