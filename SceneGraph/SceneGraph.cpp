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
#include "ShaderGenerator.h"
#include "Light.h"
#include "SceneGraph.h"

NodeResource::NodeResource(const string &name,SceneGraph *sceneGraph,const string &parent) : translation(0,0,0), rotation(0,0,0), scale(1,1,1)
{
	this->name=name;
	this->sceneGraph=sceneGraph;
	if (parent=="")
		this->parent=Node();
	else
		this->parent=sceneGraph->GetNode(parent);
}
mat4 NodeResource::GetTransformation()
{
	if (!transformationChanged)
		return transformation;
	transformationChanged=false;
	transformation.Identity();
	transformation.Translate(translation);	
	transformation.Scale(scale.x,scale.y,scale.z);	
	transformation.RotateX(rotation.x);
	transformation.RotateY(rotation.y);
	transformation.RotateZ(rotation.z);
	return transformation;
}
void NodeResource::SetTranslation(vec3 t)
{
	transformationChanged=true;
	translation=t;
}
vec3 NodeResource::GetTranslation()
{	
	return translation;
}
void NodeResource::SetRotation(vec3 r)
{
	transformationChanged=true;
	rotation=r;
}
vec3 NodeResource::GetRotation()
{
	return rotation;
}
void NodeResource::SetScale(vec3 s)
{
	transformationChanged=true;
	scale=s;
}
vec3 NodeResource::GetScale()
{
	return scale;
}
void NodeResource::Move(vec3 t)
{
	transformationChanged=true;
	translation+=t;
}
void NodeResource::Rotate(vec3 r)
{
	transformationChanged=true;
	rotation+=r;
}
void NodeResource::Scale(vec3 s)
{
	transformationChanged=true;
	scale.x*=s.x;
	scale.y*=s.y;
	scale.z*=s.z;
}
void NodeResource::AttachModel(Model model)
{
	if (model)
		attachedModels.push_back(model);
}
void NodeResource::AttachModel(const string &name)
{
	Model m=sceneGraph->GetModel(name);
	if (m)
		attachedModels.push_back(m);
}
void NodeResource::DetachModel(int i)
{
	attachedModels.erase(attachedModels.begin()+i);	
}
void NodeResource::DetachModel(const string &name)
{
	Model m=sceneGraph->GetModel(name);
	if (m)
	{
		vector<Model>::iterator i=std::find(attachedModels.begin(),attachedModels.end(),m);
		if (i!=attachedModels.end())
			attachedModels.erase(i);
	}
}
void NodeResource::AttachLight(const string &name)
{
	Light l=sceneGraph->GetLight(name);
	if (l)
		attachedLights.push_back(l);
}
void NodeResource::DetachLight(const string &name)
{
	Light l=sceneGraph->GetLight(name);
	if (l)
	{
		vector<Light>::iterator i=std::find(attachedLights.begin(),attachedLights.end(),l);
		if (i!=attachedLights.end())
			attachedLights.erase(i);
	}
}
void NodeResource::Render()
{
	if (parent)
		worldTransformation=parent->worldTransformation*GetTransformation();
	else
		worldTransformation=GetTransformation();	
	for (vector<Model>::iterator i=attachedModels.begin();i!=attachedModels.end();i++)
	{		
		sceneGraph->renderingQueue.push_back(make_pair(worldTransformation,*i));
	}
	for (vector<Light>::iterator i=attachedLights.begin();i!=attachedLights.end();i++)
	{		
		sceneGraph->activeLights.push_back(make_pair(worldTransformation,*i));
	}
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
		(*i)->Render();	
}
Node NodeResource::AddChild(const string &name)
{
	Node ret(new NodeResource(name,sceneGraph,this->name));	
	sceneGraph->nodes[name]=ret;
	children.push_back(ret);
	return ret;
}
Node NodeResource::GetChild(const string &name)
{
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
		if ((*i)->GetName()==name)
			return *i;
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
	{
		Node ret=(*i)->GetChild(name);
		if (ret)
			return ret;
	}
	return Node();
}
Node NodeResource::RemoveChild(const string &name)
{
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
		if ((*i)->GetName()==name)
		{
			Node ret=*i;
			children.erase(i);
			sceneGraph->nodes.erase(name);
			return ret;
		}
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
	{
		Node ret=(*i)->RemoveChild(name);
		if (ret)
			return ret;
	}
	return Node();
}
const string &NodeResource::GetName() const
{
	return name;
}
SceneGraph::SceneGraph()
{
	root=Node(new NodeResource("root",this,""));
	nodes["root"]=root;
}
Node SceneGraph::Root()
{
	return root;
}
void SceneGraph::Render()
{
	bool firstPass=true;
	renderingQueue.clear();
	activeLights.clear();
	root->Render();
	vector<pair<mat4,Light>>::iterator i;
	vector<pair<mat4,Model>>::iterator j;
	glEnable(GL_BLEND);
	
	for (i=activeLights.begin();i!=activeLights.end();i++)
	{				
		if (firstPass)
		{
			glDepthFunc(GL_LESS);
			glBlendFunc(GL_ONE,GL_ZERO);
			glDepthMask(true);
		}
		else
		{
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_ONE,GL_ONE);
			glDepthMask(false);
		}
		firstPass=false;
		for (j=renderingQueue.begin();j!=renderingQueue.end();j++)
		{
			Model model=j->second;
			for (vector<Material>::iterator k=model->material.begin();k!=model->material.end();k++)
			{
				RenderState rs;
				rs.FromMaterial(*k);
				if (i->second->type==DIRECTIONAL)
					rs.SetDirectionalLighting(true);				
				rs.SetFog(fog);
				Program p=shaderGenerator.GenerateProgram(rs);
				(*k)->program=p;
				Renderer::UseProgram(p);
				if (i->second->type==DIRECTIONAL)
				{
					p->SetUniform("lightDir",vec3(0,0,1).TransformNormal(i->first));
					p->SetUniform("lightDiffuse",i->second->diffuseColor);
					p->SetUniform("lightSpecular",i->second->specularColor);
					p->SetUniform("lightShininess",i->second->shininess);
				}
				if (fog)
				{
					p->SetUniform("fogColor",fogColor);
					p->SetUniform("fogDensity",fogDensity);
				}
			}
			Renderer::SetModelViewMatrix(j->first);
			Renderer::Render(model);
		}
	}
	glDepthMask(true);
	glDisable(GL_BLEND);
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
	Model m=modelManager.Create(filename);	
	models[name]=m;
	return m;
}
Light SceneGraph::AddLight(const string &name)
{
	Light l(new LightResource);	
	lights[name]=l;
	return l;
}

Model SceneGraph::GetModel(const string &name)
{
	map<string,Model>::iterator i=models.find(name);
	if (i==models.end())
		return Model();
	return i->second;
}
Light SceneGraph::GetLight(const string &name)
{
	map<string,Light>::iterator i=lights.find(name);
	if (i==lights.end())
		return Light();
	return i->second;
}
void SceneGraph::SetFogColor(vec4 &color)
{
	fogColor=color;
}
void SceneGraph::SetFogDensity(float d)
{
	fogDensity=d;
}
void SceneGraph::SetFog(bool enabled)
{
	fog=enabled;
}