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
#include "ShaderGenerator.h"
#include "Renderer.h"				
#include "Application.h"
#include "Light.h"
#include "Node.h"
#include "tinyxml.h"
#include "ModelLoaders.h"

using namespace FireCube;
Node::Node()
{	
}
Node::Node(NodeResource *nr) : nodeResource(nr)
{
}
Node::operator bool () const
{
	return nodeResource;
}
bool Node::operator== (const Node &node) const
{
	return nodeResource==node.nodeResource;
}
Node::Node(const string &name)
{
	nodeResource=boost::shared_ptr<NodeResource>(new NodeResource);
	SetName(name);
}
void Node::SetName(const string &name)
{
	nodeResource->name=name;
}
string Node::GetName()
{
	return nodeResource->name;
}
Node &Node::Parent()
{
	return nodeResource->parent;
}
mat4 Node::GetLocalTransformation()
{
	if (!nodeResource->transformationChanged)
		return nodeResource->transformation;
	nodeResource->transformationChanged=false;
	nodeResource->transformation.Identity();
	nodeResource->transformation.Translate(nodeResource->translation);	
	nodeResource->transformation.Scale(nodeResource->scale.x,nodeResource->scale.y,nodeResource->scale.z);	
	nodeResource->transformation.RotateX(nodeResource->rotation.x);
	nodeResource->transformation.RotateY(nodeResource->rotation.y);
	nodeResource->transformation.RotateZ(nodeResource->rotation.z);	
	nodeResource->transformation*=nodeResource->matTransform;
	return nodeResource->transformation;
}
mat4 Node::GetWorldTransformation()
{
	return nodeResource->worldTransformation;
}
void Node::SetWorldTransformation(mat4 mat)
{
	nodeResource->worldTransformation=mat;
}

void Node::SetTranslation(vec3 t)
{
	nodeResource->transformationChanged=true;
	nodeResource->translation=t;
}
vec3 Node::GetTranslation()
{	
	return nodeResource->translation;
}
void Node::SetRotation(vec3 r)
{
	nodeResource->transformationChanged=true;
	nodeResource->rotation=r;
}
vec3 Node::GetRotation()
{
	return nodeResource->rotation;
}
void Node::SetScale(vec3 s)
{
	nodeResource->transformationChanged=true;
	nodeResource->scale=s;
}
vec3 Node::GetScale()
{
	return nodeResource->scale;
}
void Node::SetMatrixTransformation(mat4 t)
{
	nodeResource->transformationChanged=true;
	nodeResource->matTransform=t;
}
mat4 Node::GetMatrixTransformation()
{
	return nodeResource->matTransform;
}
void Node::Move(vec3 t)
{
	nodeResource->transformationChanged=true;
	nodeResource->translation+=t;
}
void Node::Rotate(vec3 r)
{
	nodeResource->transformationChanged=true;
	nodeResource->rotation+=r;
}
void Node::Scale(vec3 s)
{
	nodeResource->transformationChanged=true;
	nodeResource->scale.x*=s.x;
	nodeResource->scale.y*=s.y;
	nodeResource->scale.z*=s.z;
}
void Node::AddGeometry(Geometry geometry)
{
	nodeResource->geometries.push_back(geometry);
}
void Node::AddLight(Light l)
{	
	nodeResource->lights.push_back(l);
}
void Node::RemoveLight(Light l)
{
	vector<Light>::iterator i=std::find(nodeResource->lights.begin(),nodeResource->lights.end(),l);
	if (i!=nodeResource->lights.end())
		nodeResource->lights.erase(i);
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
	DWORD i[12*2]={0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7};
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

	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
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
	return nodeResource->geometries;
}
vector<Light> &Node::GetLights()
{
	return nodeResource->lights;
}
void Node::Render()
{
	if (nodeResource->parent)
		SetWorldTransformation(Parent().GetWorldTransformation()*GetLocalTransformation());
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
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
		(*i).Render();	
}
Node Node::AddChild(Node node)
{
	nodeResource->children.push_back(node);
	if (node.Parent())
		node.Parent().RemoveChild(node);
	node.nodeResource->parent=*this;
	return node;
}
void Node::SetParent(Node parent)
{
	if (Parent())
		Parent().RemoveChild(*this);
	nodeResource->parent=parent;
	parent.nodeResource->children.push_back(*this);		
}
Node Node::GetChild(const string &name)
{
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
		if ((*i).GetName()==name)
			return *i;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		Node ret=(*i).GetChild(name);
		if (ret)
			return ret;
	}
	return Node(false);
}
Node Node::RemoveChild(Node node)
{
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
		if ((*i)==node)
		{			
			nodeResource->children.erase(i);
			return node;
		}
		for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
		{
			Node ret=(*i).RemoveChild(node);
			if (ret)
				return ret;
		}
		return Node(false);
}
Node Node::RemoveChild(const string &name)
{
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
		if ((*i).GetName()==name)
		{
			Node ret=*i;
			nodeResource->children.erase(i);
			return ret;
		}
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		Node ret=(*i).RemoveChild(name);
		if (ret)
			return ret;
	}
	return Node(false);
}
vector<Node> &Node::GetChildren()
{
	return nodeResource->children;
}
void Node::CreateHardNormals()
{
	for (vector<Geometry>::iterator i=GetGeometries().begin();i!=GetGeometries().end();i++)
		(*i)->CreateHardNormals();
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).CreateHardNormals();
	}
}
void Node::SetProgram(Program program)
{
	nodeResource->renderParameters.program=program;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).SetProgram(program);
	}
}
RenderParameters &Node::GetRenderParameters()
{
	return nodeResource->renderParameters;
}
Program Node::GetProgram()
{
	return nodeResource->renderParameters.program;
}
void Node::SetLighting(bool enabled)
{
	nodeResource->renderParameters.lighting=enabled;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).SetLighting(enabled);
	}
}
bool Node::GetLighting()
{
	return nodeResource->renderParameters.lighting;
}
void Node::SetFog(bool enabled)
{
	nodeResource->renderParameters.fog=enabled;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).SetFog(enabled);
	}
}
bool Node::GetFog()
{
	return nodeResource->renderParameters.fog;
}
void Node::SetFogColor(vec4 color)
{
	nodeResource->renderParameters.fogColor=color;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).SetFogColor(color);
	}
}
vec4 Node::GetFogColor()
{
	return nodeResource->renderParameters.fogColor;
}
void Node::SetFogDensity(float density)
{
	nodeResource->renderParameters.fogDensity=density;
	for (vector<Node>::iterator i=nodeResource->children.begin();i!=nodeResource->children.end();i++)
	{
		(*i).SetFogDensity(density);
	}
}
float Node::GetFogDensity()
{
	return nodeResource->renderParameters.fogDensity;
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
	string::size_type d;
	d=filename.find_last_of(".");
	//name=filename;
	ostringstream ss;
	ss<< "Created model with name:"<<filename<<endl;
	Logger::Write(ss.str());
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="3ds")
		{				
			M3dsLoader m3dsLoader;

			if (m3dsLoader.Load(filename))							
			{				
				return m3dsLoader.GenerateSceneGraph();
			}
			else
				return Node();
		}
		if (ext=="dae")
		{				
			ColladaLoader colladaLoader(filename);

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