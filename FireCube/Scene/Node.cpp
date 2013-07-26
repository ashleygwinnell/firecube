#include <sstream>

#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Geometry/Material.h"
#include "Scene/Node.h"
#include "Geometry/m3dsLoader.h"
#include "Geometry/ObjLoader.h"
#include "Geometry/ColladaLoader.h"
#include "Rendering/Shaders.h"

using namespace FireCube;

Node::Node() : parent(nullptr)
{
	rotation = mat4::identity;
	translation.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	renderParameters.lighting = true;
	renderParameters.fog = false;
	renderParameters.technique = Renderer::GetTechnique("default");
	localTransformationChanged = true;
	worldTransformationChanged = true;
	worldBoundingBoxChanged = true;
	type = NODE;
}

Node::Node(const std::string &name) : parent(nullptr)
{
	rotation = mat4::identity;
	translation.Set(0, 0, 0);
	scale.Set(1, 1, 1);	
	renderParameters.lighting = true;
	renderParameters.fog = false;
	renderParameters.technique = Renderer::GetTechnique("default");
	localTransformationChanged = true;
	worldTransformationChanged = true;
	worldBoundingBoxChanged = true;
	type = NODE;
	SetName(name);
}

Node::NodeType Node::GetType() const
{
	return type;
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
	if (!localTransformationChanged)
		return localTransformation;
	localTransformationChanged = false;
	localTransformation.Identity();
	localTransformation.Translate(translation);
	localTransformation.Scale(scale.x, scale.y, scale.z);
	localTransformation *= rotation;
	return localTransformation;
}

mat4 Node::GetWorldTransformation()
{
	if (!worldTransformationChanged)
		return worldTransformation;

	worldTransformationChanged = false;
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
	SetTransformationChanged();
}

void Node::RenderBoundingBox(vec3 color, bool onlyWithGeometry)
{
	MaterialPtr mat(new Material);
	mat->SetDiffuseColor(color);
	ShaderProperties shaderProperties;
	shaderProperties.FromMaterial(mat);
	ProgramPtr p = Renderer::GetTechnique("default")->GenerateProgram(shaderProperties);
	//Renderer::SaveModelViewMatrix();
	//Renderer::SetModelViewMatrix(Renderer::GetCamera()->GetViewMatrix());
	RenderBoundingBox(mat, p, onlyWithGeometry);
	//Renderer::RestoreModelViewMatrix();
}

void Node::Render()
{
	/*for (vector<Model>::iterator i=attachedModels.begin();i!=attachedModels.end();i++)
	{
		sceneGraph->renderingQueue.push_back(make_pair(worldTransformation,*i));
	}
	for (vector<Light>::iterator i=attachedLights.begin();i!=attachedLights.end();i++)
	{
		sceneGraph->activeLights.push_back(make_pair(worldTransformation,*i));
	}*/
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
		(*i)->Render();
}

NodePtr Node::AddChild(NodePtr node)
{
	children.push_back(node);
	// If the node has a parent, remove it from the parent's children list
	if (node->GetParent())
		node->GetParent()->RemoveChild(node);
	node->parent = this;
	SetBoundingBoxChanged();
	return node;
}

void Node::SetParent(NodePtr parent)
{
	if (GetParent())	
		GetParent()->RemoveChild(shared_from_this());		
	
	this->parent = parent.get();
	if (parent)
	{
		parent->children.push_back(shared_from_this());
		parent->SetBoundingBoxChanged();
	}
}

NodePtr Node::GetChild(const std::string &name)
{
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
		if ((*i)->GetName() == name)
			return *i;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr ret = (*i)->GetChild(name);
		if (ret)
			return ret;
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
			SetBoundingBoxChanged();
			return node;
		}
	}
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr ret = (*i)->RemoveChild(node);
		if (ret)
			return ret;
	}
	return NodePtr();
}

NodePtr Node::RemoveChild(const std::string &name)
{
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
		if ((*i)->GetName() == name)
		{
			NodePtr ret = *i;
			SetBoundingBoxChanged();
			children.erase(i);
			return ret;
		}
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr ret = (*i)->RemoveChild(name);
		if (ret)
			return ret;
	}
	return NodePtr();
}

std::vector<NodePtr> &Node::GetChildren()
{
	return children;
}

RenderParameters &Node::GetRenderParameters()
{
	return renderParameters;
}

void Node::SetProgram(ProgramPtr program)
{
	renderParameters.program = program;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetProgram(program);
	}
}

ProgramPtr Node::GetProgram() const
{
	return renderParameters.program;
}

void Node::SetTechnique(const std::string &name)
{
	renderParameters.technique = Renderer::GetTechnique(name);
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetTechnique(name);
	}
}

TechniquePtr Node::GetTechnique() const
{
	return renderParameters.technique;
}

void Node::SetLighting(bool enabled)
{
	renderParameters.lighting = enabled;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetLighting(enabled);
	}
}

bool Node::GetLighting() const
{
	return renderParameters.lighting;
}

void Node::SetFog(bool enabled)
{
	renderParameters.fog = enabled;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetFog(enabled);
	}
}

bool Node::GetFog() const
{
	return renderParameters.fog;
}

void Node::SetFogColor(const vec4 &color)
{
	renderParameters.fogColor = color;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetFogColor(color);
	}
}

vec4 Node::GetFogColor() const
{
	return renderParameters.fogColor;
}

void Node::SetFogDensity(float density)
{
	renderParameters.fogDensity = density;
	for (std::vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->SetFogDensity(density);
	}
}

float Node::GetFogDensity() const
{
	return renderParameters.fogDensity;
}

NodePtr Node::Clone() const
{
	NodePtr ret(new Node);
	ret->name = name;
	ret->renderParameters = renderParameters;
	ret->translation = translation;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->localTransformation = localTransformation;
	ret->localTransformationChanged = localTransformationChanged;
	ret->worldTransformation = worldTransformation;
	ret->worldTransformationChanged = worldTransformationChanged;
	ret->worldBoundingBox = worldBoundingBox;
	ret->worldBoundingBoxChanged = worldBoundingBoxChanged;
	for (std::vector<NodePtr>::const_iterator i = children.begin(); i != children.end(); i++)
	{
		NodePtr c = (*i)->Clone();
		c->SetParent(ret);
	}
	return ret;
}

void Node::SetTransformationChanged()
{
	if (localTransformationChanged)
		return;

	worldTransformationChanged = true;
	localTransformationChanged = true;
	SetBoundingBoxChanged();
	for (unsigned int i = 0; i < children.size(); i++)
	{
		children[i]->SetTransformationChanged();
	}
}

void Node::SetBoundingBoxChanged()
{
	if (worldBoundingBoxChanged)
		return;
	worldBoundingBoxChanged = true;
	worldBoundingBoxRenderingChanged = true;
	Node *p = parent;
	while (p)
	{
		if (p->worldBoundingBoxChanged)
			break;
		p->worldBoundingBoxChanged = true;
		p->worldBoundingBoxRenderingChanged = true;
		p = p->parent;
	}
}

void Node::UpdateWorldBoundingBox()
{	
	if (!worldBoundingBoxChanged)
		return;

	worldBoundingBox = GetLocalBoundingBox();	
	worldBoundingBox.Transform(GetWorldTransformation());	
	for (auto i = children.begin(); i != children.end(); ++i)
	{
		worldBoundingBox.Expand((*i)->GetWorldBoundingBox());
	}    
	worldBoundingBoxChanged = false;
}

BoundingBox Node::GetWorldBoundingBox()
{
	UpdateWorldBoundingBox();
	return worldBoundingBox;
}

void Node::RenderBoundingBox(MaterialPtr material, ProgramPtr program, bool onlyWithGeometry)
{
	/*if (!onlyWithGeometry || (onlyWithGeometry && geometry))
	{
		PrepareRenderBoundingBox();
		bboxVBuffer->SetVertexAttribute(0, 3, 0, 0);
		bboxIBuffer->SetIndexStream();
		Renderer::UseProgram(program);
		Renderer::UseMaterial(program, material);
		Renderer::RenderIndexStream(LINES, 24);
	}

	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->RenderBoundingBox(material, program, onlyWithGeometry);*/
}

void Node::PrepareRenderBoundingBox()
{
	if (!worldBoundingBoxRenderingChanged)
		return;

	if (!bboxVBuffer)
	{
		bboxVBuffer = BufferPtr(new Buffer);
		bboxVBuffer->Create();
	}
	if (!bboxIBuffer)
	{
		bboxIBuffer = BufferPtr(new Buffer);
		bboxIBuffer->Create();
	}
	BoundingBox bbox = GetWorldBoundingBox();
	float w = bbox.GetWidth();
	float h = bbox.GetHeight();
	float d = bbox.GetDepth();
	vec3 v[8] = {bbox.GetMin(),
				 bbox.GetMin() + vec3(w, 0, 0),
				 bbox.GetMin() + vec3(w, 0, d),
				 bbox.GetMin() + vec3(0, 0, d),
				 bbox.GetMin() + vec3(0, h, 0),
				 bbox.GetMin() + vec3(w, h, 0),
				 bbox.GetMin() + vec3(w, h, d),
				 bbox.GetMin() + vec3(0, h, d)
				};
	unsigned int i[12 * 2] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};


	bboxVBuffer->LoadData(v, sizeof(vec3) * 8, STREAM);
	bboxIBuffer->LoadIndexData(i, 24, STREAM);
	worldBoundingBoxRenderingChanged = false;
}

NodePtr FIRECUBE_API FireCube::LoadMesh(const std::string &filename, ModelLoadingOptions options)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return NodePtr();
	std::string::size_type d;
	d = filename.find_last_of(".");	
	std::ostringstream ss;
	ss << "Created model with name:" << filename;
	Logger::Write(Logger::LOG_INFO, ss.str());
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		if (ext == "3ds")
		{
			M3dsLoader m3dsLoader;

			if (m3dsLoader.Load(file, options))
			{
				return m3dsLoader.GenerateSceneGraph();
			}
			else
				return NodePtr();
		}
		else if (ext == "dae")
		{
			ColladaLoader colladaLoader(file);

			if (colladaLoader.Load(options))
			{
				return colladaLoader.GenerateSceneGraph();
			}
			else
				return NodePtr();
		}
		else if (ext == "obj")
		{
			ObjLoader objLoader;

			objLoader.Load(file, options);
			return objLoader.GenerateSceneGraph();			
		}
	}
	return NodePtr();
}

vec3 Node::GetWorldPosition()
{
	return GetWorldTransformation() * vec3(0.0f, 0.0f, 0.0f);
}

void Node::PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera)
{
	for (auto i = children.begin(); i != children.end(); ++i)
		(*i)->PopulateRenderQueue(renderQueue, camera);
}

BoundingBox Node::GetLocalBoundingBox() const
{
	return BoundingBox();
}