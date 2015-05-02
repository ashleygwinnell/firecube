#pragma once

#include "FireCube.h"

class NodeDescriptor;

class SceneDescriptor
{
public:
	SceneDescriptor();
	~SceneDescriptor();
	void Init(FireCube::Engine *engine);
	FireCube::Scene *GetScene();
	NodeDescriptor *NodeToNodeDescriptor(FireCube::Node *node);
	void NodeAdded(NodeDescriptor *node);
	void NodeRemoved(NodeDescriptor *node);
	NodeDescriptor *GetRootDescriptor();
private:
	FireCube::Scene *scene;
	NodeDescriptor *rootDescriptor;
	std::map<FireCube::Node *, NodeDescriptor *> nodeDescriptorsMap;
};

class NodeDescriptor
{
public:
	~NodeDescriptor();
	NodeDescriptor(SceneDescriptor *sceneDescriptor, FireCube::Node *node);
	NodeDescriptor(FireCube::Engine *engine, SceneDescriptor *sceneDescriptor, const std::string &name = "");
	NodeDescriptor *CreateChild(const std::string &name = "");
	FireCube::Node *GetNode();
	void SetParent(NodeDescriptor *parent);
	NodeDescriptor *GetParent();
	void Remove();
private:
	FireCube::SharedPtr<FireCube::Node> node;
	SceneDescriptor *scene;
	std::vector<NodeDescriptor *> children;
	SceneDescriptor *sceneDescriptor;
	NodeDescriptor *parent;
};