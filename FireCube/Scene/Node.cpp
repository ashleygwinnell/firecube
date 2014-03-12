#include <sstream>
#include "Scene/Node.h"
#include "Utils/Logger.h"

#include "Utils/Filesystem.h"
#include "Geometry/ColladaLoader.h"
#include "Geometry/m3dsLoader.h"
#include "Geometry/ObjLoader.h"
#include "Core/Engine.h"

using namespace FireCube;

Node::Node(Engine *engine) : Object(engine), parent(nullptr)
{
	rotation = mat4::IDENTITY;
	translation.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	worldTransformation.Identity();
	transformationChanged = false;	
}

Node::Node(Engine *engine, const std::string &name) : Object(engine), parent(nullptr)
{
	rotation = mat4::IDENTITY;
	translation.Set(0, 0, 0);
	scale.Set(1, 1, 1);		
	worldTransformation.Identity();
	transformationChanged = false;	
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
	if (!transformationChanged)
		return worldTransformation;
	transformationChanged = false;
	if (parent)
		worldTransformation = parent->GetWorldTransformation() * GetLocalTransformation();			
	else
		worldTransformation = GetLocalTransformation();
	
	return worldTransformation;	
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

NodePtr Node::AddChild(NodePtr node)
{
	children.push_back(node);
	// If the node has a parent, remove it from the parent's children list
	if (node->GetParent())
		node->GetParent()->RemoveChild(node);
	node->parent = this;
	node->scene = scene;	
	return node;
}

NodePtr Node::CreateChild(const std::string &name)
{
	NodePtr child(new Node(engine, name));
	AddChild(child);
	return child;
}

void Node::SetParent(NodePtr parent)
{
	if (GetParent())	
		GetParent()->RemoveChild(shared_from_this());		
	
	this->parent = parent.get();
	if (parent)
	{
		scene = parent->scene;
		parent->children.push_back(shared_from_this());		
	}
}

NodePtr Node::GetChild(const std::string &name, bool recursive)
{
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		if ((*i)->GetName() == name)
			return *i;
		if (recursive)
		{
			NodePtr node = (*i)->GetChild(name, true);
			if (node)
				return node;
		}
	}
	
	return NodePtr();
}

NodePtr Node::RemoveChild(NodePtr node)
{
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		if ((*i) == node)
		{
			children.erase(i);			
			return node;
		}
	}
	
	return NodePtr();
}

NodePtr Node::RemoveChild(const std::string &name)
{
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		if ((*i)->GetName() == name)
		{
			NodePtr ret = *i;			
			children.erase(i);
			return ret;
		}
	}
	
	return NodePtr();
}

void Node::RemoveAllChildren()
{
	children.clear();
}

std::vector<NodePtr> &Node::GetChildren()
{
	return children;
}


NodePtr Node::Clone() const
{
	NodePtr ret(new Node(engine));
	//TODO: Implement
	ret->name = name;
	ret->translation = translation;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->localTransformation = localTransformation;
	ret->transformationChanged = transformationChanged;
	ret->worldTransformation = worldTransformation;	
	for (std::vector<NodePtr>::const_iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr c = (*i)->Clone();
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
		LOGERROR("Attemp to add a component which already has a parent node.");
	}
	else
	{
		components.push_back(ComponentPtr(component));
		component->SetNode(this);
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
	}

	components.clear();
}

void Node::Load(const std::string &filename, ModelLoadingOptions options)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return;
	std::string::size_type d;
	d = filename.find_last_of(".");	
	std::ostringstream ss;
	ss << "Created model with name:" << filename;
	LOGINFO(ss.str());	
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		ModelLoader *modelLoader = nullptr;
		if (ext == "3ds")
			modelLoader = new M3dsLoader(engine);		
		else if (ext == "dae")		
			modelLoader = new ColladaLoader(engine);
		else if (ext == "obj")
			modelLoader = new ObjLoader(engine);

		if (modelLoader->Load(file, options))
		{
			modelLoader->GenerateScene(engine->GetRenderer(), this);
			
		}		

		delete modelLoader;
	}	
}