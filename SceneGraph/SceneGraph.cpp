#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <queue>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "SceneGraph.h"

NodeResource::NodeResource(const string &name,SceneGraph *sceneGraph,NodeResource *parent)
{
	this->name=name;
	this->sceneGraph=sceneGraph;
	this->parent=sceneGraph->GetNode(parent);
}
mat4 &NodeResource::GetTransformation()
{
	return transformation;
}
void NodeResource::SetTransformation(const mat4 &t)
{	
	transformation=t;
}
void NodeResource::AttachModel(Model model)
{
	if (model)
		attachedModels.push_back(model);
}
void NodeResource::DetachModel(int i)
{
	attachedModels.erase(attachedModels.begin()+i);	
}
void NodeResource::Render()
{
	Renderer::SaveModelViewMatrix();
	Renderer::MultiplyModelViewMatrix(transformation);
	for (vector<Model>::iterator i=attachedModels.begin();i!=attachedModels.end();i++)
	{
		Renderer::Render(*i);
	}
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
		(*i)->Render();
	Renderer::RestoreModelViewMatrix();
}
Node NodeResource::AddChild(const string &name,const mat4 &t)
{
	Node ret(new NodeResource(name,sceneGraph,this));
	ret->SetTransformation(t);	
	sceneGraph->nodes[name]=ret;
	children.push_back(ret);
	return ret;
}
Node NodeResource::GetChild(const string &name)
{
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
		if ((*i)->GetName()==name)
			return *i;
	return Node();
}
const string &NodeResource::GetName() const
{
	return name;
}
SceneGraph::SceneGraph()
{
	root=Node(new NodeResource("root",this,NULL));
	nodes["root"]=root;
}
Node SceneGraph::Root()
{
	return root;
}
void SceneGraph::Render()
{
	root->Render();
}
Node SceneGraph::GetNode(NodeResource *node)
{
	map<string,Node>::iterator i=nodes.find(node->name);
	if (i==nodes.end())
		return Node();
	return i->second;
}
Node SceneGraph::GetNode(const string &name)
{
	map<string,Node>::iterator i=nodes.find(name);
	if (i==nodes.end())
		return Node();
	return i->second;
}
Model SceneGraph::AddModel(const string &name,const string &filename)
{
	models[name]=modelManager.Create(filename);
	return models[name];
}
Model SceneGraph::GetModel(const string &name)
{
	map<string,Model>::iterator i=models.find(name);
	if (i==models.end())
		return Model();
	return i->second;
}