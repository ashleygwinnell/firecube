#include "FireCube.h"
using namespace FireCube;

#include "SceneDescriptor.h"

SceneDescriptor::SceneDescriptor()
{

}

SceneDescriptor::~SceneDescriptor()
{
	delete scene;	
	delete rootDescriptor;
}

void SceneDescriptor::Init(Engine *engine)
{
	scene = new Scene(engine);	
	rootDescriptor = new NodeDescriptor(this, scene->GetRootNode()->CreateChild("root"));
	NodeAdded(rootDescriptor);
}

Scene *SceneDescriptor::GetScene()
{
	return scene;
}

NodeDescriptor *SceneDescriptor::NodeToNodeDescriptor(Node *node)
{
	if (nodeDescriptorsMap.find(node) != nodeDescriptorsMap.end())
	{
		return nodeDescriptorsMap[node];
	}

	return nullptr;
}

void SceneDescriptor::NodeAdded(NodeDescriptor *node)
{
	nodeDescriptorsMap[node->GetNode()] = node;
}

void SceneDescriptor::NodeRemoved(NodeDescriptor *node)
{
	if (nodeDescriptorsMap.find(node->GetNode()) != nodeDescriptorsMap.end())
	{
		nodeDescriptorsMap.erase(nodeDescriptorsMap.find(node->GetNode()));
	}
}


NodeDescriptor *SceneDescriptor::GetRootDescriptor()
{
	return rootDescriptor;
}

NodeDescriptor::NodeDescriptor(SceneDescriptor *sceneDescriptor, FireCube::Node *node) : sceneDescriptor(sceneDescriptor), node(node), parent(nullptr)
{

}

NodeDescriptor::NodeDescriptor(Engine *engine, SceneDescriptor *sceneDescriptor, const std::string &name) : sceneDescriptor(sceneDescriptor), node(new Node(engine, name)), parent(nullptr)
{
	sceneDescriptor->NodeAdded(this);
}

NodeDescriptor::~NodeDescriptor()
{
	sceneDescriptor->NodeRemoved(this);
}

NodeDescriptor *NodeDescriptor::CreateChild(const std::string &name)
{
	auto childNode = node->CreateChild(name);
	NodeDescriptor *childDescriptor = new NodeDescriptor(sceneDescriptor, childNode);
	children.push_back(childDescriptor);
	sceneDescriptor->NodeAdded(childDescriptor);
	return childDescriptor;
}

FireCube::Node *NodeDescriptor::GetNode()
{
	return node;
}

void NodeDescriptor::SetParent(NodeDescriptor *parent)
{
	if (this->parent)
	{
		this->parent->children.erase(std::remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
	}

	this->parent = parent;
	this->node->SetParent(parent->GetNode());
	parent->children.push_back(this);
}

void NodeDescriptor::Remove()
{
	if (this->parent)
	{
		this->parent->children.erase(std::remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
		this->node->Remove();
		this->parent = nullptr;
	}
}