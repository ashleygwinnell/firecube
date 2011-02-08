#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include <GL/GLU.h>
#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Application.h"
#include "Light.h"
#include "Node.h"
#include "tinyxml.h"
#include "ModelLoaders.h"

using namespace FireCube;
void Light::Create()
{
	resource=boost::shared_ptr<LightResource>(new LightResource);
}
void Light::SetType(LightType type)
{
	resource->type=type;
}
LightType Light::GetType()
{
	return resource->type;
}
void Light::SetAmbientColor(vec4 color)
{
	resource->ambientColor=color;
}
vec4 Light::GetAmbientColor()
{
	return resource->ambientColor;
}
void Light::SetDiffuseColor(vec4 color)
{
	resource->diffuseColor=color;
}
vec4 Light::GetDiffuseColor()
{
	return resource->diffuseColor;
}
void Light::SetSpecularColor(vec4 color)
{
	resource->specularColor=color;
}
vec4 Light::GetSpecularColor()
{
	return resource->specularColor;
}
Light::operator bool () const
{
	return resource;
}
bool Light::operator== (const Light &light) const
{
	return light.resource==resource;
}
Node::Node()
{	
}
Node::Node(NodeResource *nr) : resource(nr)
{
}
Node::operator bool () const
{
	return resource;
}
bool Node::operator== (const Node &node) const
{
	return resource==node.resource;
}
Node::Node(const string &name)
{
	resource=boost::shared_ptr<NodeResource>(new NodeResource);
	SetName(name);
}
void Node::SetName(const string &name)
{
	resource->name=name;
}
string Node::GetName()
{
	return resource->name;
}
Node &Node::GetParent()
{
	return resource->parent;
}
mat4 Node::GetLocalTransformation()
{
	if (!resource->transformationChanged)
		return resource->transformation;
	resource->transformationChanged=false;
	resource->transformation.Identity();
	resource->transformation.Translate(resource->translation);	
	resource->transformation.Scale(resource->scale.x,resource->scale.y,resource->scale.z);	
	resource->transformation.RotateX(resource->rotation.x);
	resource->transformation.RotateY(resource->rotation.y);
	resource->transformation.RotateZ(resource->rotation.z);	
	resource->transformation*=resource->matTransform;
	return resource->transformation;
}
mat4 Node::GetWorldTransformation()
{
	return resource->worldTransformation;
}
void Node::SetWorldTransformation(mat4 mat)
{
	resource->worldTransformation=mat;
}

void Node::SetTranslation(vec3 t)
{
	resource->transformationChanged=true;
	resource->translation=t;
}
vec3 Node::GetTranslation()
{	
	return resource->translation;
}
void Node::SetRotation(vec3 r)
{
	resource->transformationChanged=true;
	resource->rotation=r;
}
vec3 Node::GetRotation()
{
	return resource->rotation;
}
void Node::SetScale(vec3 s)
{
	resource->transformationChanged=true;
	resource->scale=s;
}
vec3 Node::GetScale()
{
	return resource->scale;
}
void Node::SetMatrixTransformation(mat4 t)
{
	resource->transformationChanged=true;
	resource->matTransform=t;
}
mat4 Node::GetMatrixTransformation()
{
	return resource->matTransform;
}
void Node::Move(vec3 t)
{
	resource->transformationChanged=true;
	resource->translation+=t;
}
void Node::Rotate(vec3 r)
{
	resource->transformationChanged=true;
	resource->rotation+=r;
}
void Node::Scale(vec3 s)
{
	resource->transformationChanged=true;
	resource->scale.x*=s.x;
	resource->scale.y*=s.y;
	resource->scale.z*=s.z;
}
void Node::AddGeometry(Geometry geometry)
{
	resource->geometries.push_back(geometry);
}
void Node::AddLight(Light light)
{	
	resource->lights.push_back(light);
}
void Node::RemoveLight(Light light)
{
	vector<Light>::iterator i=std::find(resource->lights.begin(),resource->lights.end(),light);
	if (i!=resource->lights.end())
		resource->lights.erase(i);
}
void Node::RenderBoundingBox()
{
	/*float w=bbox.GetWidth();
	float h=bbox.GetHeight();
	float d=bbox.GetDepth();
	vec3 v[8]={bbox.bmin,
			   bbox.bmin+vec3(w,0,0),
			   bbox.bmin+vec3(w,0,d),
			   bbox.bmin+vec3(0,0,d),
			   bbox.bmin+vec3(0,h,0),
			   bbox.bmin+vec3(w,h,0),
			   bbox.bmin+vec3(w,h,d),
			   bbox.bmin+vec3(0,h,d)};
	unsigned int i[12*2]={0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7};
	float color[4]={0.0f,1.0f,0.0f,1.0f};
	if (parent)
		worldTransformation=parent->worldTransformation*GetTransformation();
	else
		worldTransformation=GetTransformation();

	Buffer vb(new BufferResource);
	Buffer ib(new BufferResource);
	vb->Create();
	ib->Create();
	vb->LoadData(v,sizeof(vec3)*8,STATIC);
	ib->LoadIndexData(i,24,STATIC);
	vb->SetVertexStream(3);
	ib->SetIndexStream();
	RenderState rs;
	Program p(new ProgramResource);
	p=sceneGraph->shaderGenerator.GenerateProgram(rs);
	Renderer::UseProgram(p);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,color);
	Renderer::SetModelViewMatrix(worldTransformation);
	Renderer::RenderIndexStream(LINES,24);*/

	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		(*i).RenderBoundingBox();	
}
void Node::UpdateBoundingBox()
{
	/*bbox=BoundingBox();
	for (vector<Node>::iterator i=children.begin();i!=children.end();i++)
	{
		bbox.Expand((*i)->bbox);
	}
	for (vector<Model>::iterator i=attachedModels.begin();i!=attachedModels.end();i++)
	{
		bbox.Expand((*i)->GetBoundingBox());
	}
	
	if (parent)
		parent->UpdateBoundingBox();*/

}

vector<Geometry> &Node::GetGeometries()
{
	return resource->geometries;
}
vector<Light> &Node::GetLights()
{
	return resource->lights;
}
void Node::Render()
{
	if (resource->parent)
		SetWorldTransformation(GetParent().GetWorldTransformation()*GetLocalTransformation());
	else
		SetWorldTransformation(GetLocalTransformation());
	/*for (vector<Model>::iterator i=attachedModels.begin();i!=attachedModels.end();i++)
	{		
		sceneGraph->renderingQueue.push_back(make_pair(worldTransformation,*i));
	}
	for (vector<Light>::iterator i=attachedLights.begin();i!=attachedLights.end();i++)
	{		
		sceneGraph->activeLights.push_back(make_pair(worldTransformation,*i));
	}*/
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		(*i).Render();	
}
Node Node::AddChild(Node node)
{
	resource->children.push_back(node);
	if (node.GetParent())
		node.GetParent().RemoveChild(node);
	node.resource->parent=*this;
	return node;
}
void Node::SetParent(Node parent)
{
	if (GetParent())
		GetParent().RemoveChild(*this);
	resource->parent=parent;
	if (parent)
		parent.resource->children.push_back(*this);		
}
Node Node::GetChild(const string &name)
{
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		if ((*i).GetName()==name)
			return *i;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		Node ret=(*i).GetChild(name);
		if (ret)
			return ret;
	}
	return Node();
}
Node Node::RemoveChild(Node node)
{
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		if ((*i)==node)
		{			
			resource->children.erase(i);
			return node;
		}
		for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		{
			Node ret=(*i).RemoveChild(node);
			if (ret)
				return ret;
		}
		return Node();
}
Node Node::RemoveChild(const string &name)
{
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
		if ((*i).GetName()==name)
		{
			Node ret=*i;
			resource->children.erase(i);
			return ret;
		}
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		Node ret=(*i).RemoveChild(name);
		if (ret)
			return ret;
	}
	return Node();
}
vector<Node> &Node::GetChildren()
{
	return resource->children;
}
void Node::CreateHardNormals()
{
	for (vector<Geometry>::iterator i=GetGeometries().begin();i!=GetGeometries().end();i++)
		(*i).CreateHardNormals();
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).CreateHardNormals();
	}
}
RenderParameters &Node::GetRenderParameters()
{
	return resource->renderParameters;
}
void Node::SetProgram(Program program)
{
	resource->renderParameters.program=program;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).SetProgram(program);
	}
}
Program Node::GetProgram()
{
	return resource->renderParameters.program;
}
void Node::SetTechnique(const string &name)
{
	resource->renderParameters.technique=Renderer::GetTechnique(name);
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		i->SetTechnique(name);
	}
}
Technique Node::GetTechnique() const
{
	return resource->renderParameters.technique;
}
void Node::SetLighting(bool enabled)
{
	resource->renderParameters.lighting=enabled;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).SetLighting(enabled);
	}
}
bool Node::GetLighting()
{
	return resource->renderParameters.lighting;
}
void Node::SetFog(bool enabled)
{
	resource->renderParameters.fog=enabled;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).SetFog(enabled);
	}
}
bool Node::GetFog()
{
	return resource->renderParameters.fog;
}
void Node::SetFogColor(vec4 color)
{
	resource->renderParameters.fogColor=color;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).SetFogColor(color);
	}
}
vec4 Node::GetFogColor()
{
	return resource->renderParameters.fogColor;
}
void Node::SetFogDensity(float density)
{
	resource->renderParameters.fogDensity=density;
	for (vector<Node>::iterator i=resource->children.begin();i!=resource->children.end();i++)
	{
		(*i).SetFogDensity(density);
	}
}
float Node::GetFogDensity()
{
	return resource->renderParameters.fogDensity;
}
NodeResource::NodeResource()
{
	rotation.Set(0,0,0);
	translation.Set(0,0,0);
	scale.Set(1,1,1);
	renderParameters.lighting=true;
	renderParameters.fog=false;
}
Node FIRECUBE_API FireCube::LoadMesh(const string &filename)
{
	string file=Application::SearchForFileName(filename);
	if (file.empty())
		return Node();
	string::size_type d;
	d=filename.find_last_of(".");
	//name=filename;
	ostringstream ss;
	ss<< "Created model with name:"<<filename;
	Logger::Write(Logger::LOG_INFO, ss.str());
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="3ds")
		{				
			M3dsLoader m3dsLoader;

			if (m3dsLoader.Load(file))
			{
				return m3dsLoader.GenerateSceneGraph();
			}
			else
				return Node();
		}
		if (ext=="dae")
		{
			ColladaLoader colladaLoader(file);

			if (colladaLoader.Load())
			{
				return colladaLoader.GenerateSceneGraph();
			}
			else
				return Node();
		}
	}
	return Node();
}