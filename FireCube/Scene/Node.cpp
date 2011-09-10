#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/GLee.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Dependencies/tinyxml.h"
#include "Geometry/ModelLoaders.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shaders.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"

using namespace FireCube;
void Light::SetType(LightType type)
{
    this->type = type;
}
LightType Light::GetType() const
{
    return type;
}
void Light::SetAmbientColor(const vec4 &color)
{
    ambientColor = color;
}
vec4 Light::GetAmbientColor() const
{
    return ambientColor;
}
void Light::SetDiffuseColor(const vec4 &color)
{
    diffuseColor = color;
}
vec4 Light::GetDiffuseColor() const
{
    return diffuseColor;
}
void Light::SetSpecularColor(const vec4 &color)
{
    specularColor = color;
}
vec4 Light::GetSpecularColor() const
{
    return specularColor;
}
bool Light::operator == (const Light &other) const
{
    return ambientColor == other.ambientColor && diffuseColor == other.diffuseColor && specularColor == other.specularColor && type == other.type;
}
Node::Node() : parent(NULL)
{
    rotation.Set(0, 0, 0);
    translation.Set(0, 0, 0);
    scale.Set(1, 1, 1);
	matTransform.Identity();
    renderParameters.lighting = true;
    renderParameters.fog = false;
	renderParameters.technique = Renderer::GetTechnique("default");
    localTransformationChanged = true;
    worldTransformationChanged = true;
    worldBoundingBoxChanged = true;
}
Node::Node(const string &name) : parent(NULL)
{
    rotation.Set(0, 0, 0);
    translation.Set(0, 0, 0);
    scale.Set(1, 1, 1);
	matTransform.Identity();
    renderParameters.lighting = true;
    renderParameters.fog = false;
	renderParameters.technique = Renderer::GetTechnique("default");
    localTransformationChanged = true;
    worldTransformationChanged = true;
    worldBoundingBoxChanged = true;
    SetName(name);
}
void Node::SetName(const string &name)
{
    this->name = name;
}
string Node::GetName() const
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
    localTransformation.RotateX(rotation.x);
    localTransformation.RotateY(rotation.y);
    localTransformation.RotateZ(rotation.z);
    localTransformation *= matTransform;
    return localTransformation;
}
mat4 Node::GetWorldTransformation()
{
    UpdateWorldTransformation();
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
void Node::SetRotation(const vec3 &r)
{
    rotation = r;
    SetTransformationChanged();
}
vec3 Node::GetRotation() const
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
void Node::SetMatrixTransformation(const mat4 &t)
{
    matTransform = t;
    SetTransformationChanged();
}
mat4 Node::GetMatrixTransformation() const
{
    return matTransform;
}
void Node::Move(const vec3 &t)
{
    translation += t;
    SetTransformationChanged();
}
void Node::Rotate(const vec3 &r)
{
    rotation += r;
    SetTransformationChanged();
}
void Node::Scale(const vec3 &s)
{
    scale.x *= s.x;
    scale.y *= s.y;
    scale.z *= s.z;
    SetTransformationChanged();
}
void Node::SetGeometry(GeometryPtr geometry)
{
    this->geometry = geometry;
	SetBoundingBoxChanged();
}
void Node::AddLight(const Light &light)
{
    lights.push_back(light);
}
void Node::RemoveLight(const Light &light)
{
    vector<Light>::iterator i = std::find(lights.begin(), lights.end(), light);
    if (i != lights.end())
        lights.erase(i);
}
void Node::RenderBoundingBox(vec3 color, bool onlyWithGeometry)
{
    MaterialPtr mat(new Material);
    mat->SetDiffuseColor(color);
    RenderState rs;
    rs.FromMaterial(mat);
    ProgramPtr p = Renderer::GetTechnique("default")->GenerateProgram(rs);
    Renderer::SaveModelViewMatrix();
    Renderer::SetModelViewMatrix(Renderer::GetCamera()->GetViewMatrix());
    RenderBoundingBox(mat, p, onlyWithGeometry);
    Renderer::RestoreModelViewMatrix();
}

GeometryPtr Node::GetGeometry()
{
    return geometry;
}
vector<Light> &Node::GetLights()
{
    return lights;
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
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Render();
}
NodePtr Node::AddChild(NodePtr node)
{
    children.push_back(node);
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
        parent->children.push_back(shared_from_this());
}
NodePtr Node::GetChild(const string &name)
{
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i)->GetName() == name)
            return *i;
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
    {
        NodePtr ret = (*i)->GetChild(name);
        if (ret)
            return ret;
    }
    return NodePtr();
}
NodePtr Node::RemoveChild(NodePtr node)
{
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i) == node)
        {
            children.erase(i);
			SetBoundingBoxChanged();
            return node;
        }
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
    {
        NodePtr ret = (*i)->RemoveChild(node);
        if (ret)
            return ret;
    }
    return NodePtr();
}
NodePtr Node::RemoveChild(const string &name)
{
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i)->GetName() == name)
        {
            NodePtr ret = *i;
			SetBoundingBoxChanged();
            children.erase(i);
            return ret;
        }
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
    {
        NodePtr ret = (*i)->RemoveChild(name);
        if (ret)
            return ret;
    }
    return NodePtr();
}
vector<NodePtr> &Node::GetChildren()
{
    return children;
}
void Node::CreateHardNormals()
{
    if (geometry)
        geometry->CreateHardNormals();
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->CreateHardNormals();
    }
}
RenderParameters &Node::GetRenderParameters()
{
    return renderParameters;
}
void Node::SetProgram(ProgramPtr program)
{
    renderParameters.program = program;
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetProgram(program);
    }
}
ProgramPtr Node::GetProgram() const
{
    return renderParameters.program;
}
void Node::SetTechnique(const string &name)
{
    renderParameters.technique = Renderer::GetTechnique(name);
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    ret->geometry = geometry;
    ret->lights = lights;
    ret->matTransform = matTransform;
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
    for (vector<NodePtr>::const_iterator i = children.begin(); i != children.end(); i++)
    {
        NodePtr c = (*i)->Clone();
        c->SetParent(ret);
    }
    return ret;
}
void Node::SetTransformationChanged()
{
    worldTransformationChanged = true;
    localTransformationChanged = true;
    SetBoundingBoxChanged();
    for (unsigned int i = 0; i < children.size(); i++)
    {
        children[i]->SetTransformationChanged();
    }
}
void Node::UpdateWorldTransformation()
{
    if (!worldTransformationChanged)
        return;

    if (parent)
    {
        worldTransformation = parent->GetWorldTransformation() * GetLocalTransformation();
        worldTransformationChanged = false;
    }
    else
    {
        worldTransformation = GetLocalTransformation();
        worldTransformationChanged = false;
    }
}
void Node::SetBoundingBoxChanged()
{
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

    worldBoundingBox = BoundingBox();
    if (geometry)
    {
        worldBoundingBox = geometry->GetBoundingBox();
        worldBoundingBox.Transform(GetWorldTransformation());
    }
    for (vector<NodePtr>::iterator i = children.begin(); i != children.end(); i++)
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
    if (!onlyWithGeometry || (onlyWithGeometry && geometry))
    {
        PrepareRenderBoundingBox();
        bboxVBuffer->SetVertexStream(3);
        bboxIBuffer->SetIndexStream();
        Renderer::UseProgram(program);
        Renderer::UseMaterial(material);
        Renderer::RenderIndexStream(LINES, 24);
    }

    for (unsigned int i = 0; i < children.size(); i++)
        children[i]->RenderBoundingBox(material, program, onlyWithGeometry);
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
NodePtr FIRECUBE_API FireCube::LoadMesh(const string &filename)
{
    string file = Filesystem::SearchForFileName(filename);
    if (file.empty())
        return NodePtr();
    string::size_type d;
    d = filename.find_last_of(".");
    //name=filename;
    ostringstream ss;
    ss << "Created model with name:" << filename;
    Logger::Write(Logger::LOG_INFO, ss.str());
    if (d != string::npos)
    {
        string ext = ToLower(filename.substr(d + 1));
        if (ext == "3ds")
        {
            M3dsLoader m3dsLoader;

            if (m3dsLoader.Load(file))
            {
                return m3dsLoader.GenerateSceneGraph();
            }
            else
                return NodePtr();
        }
        else if (ext == "dae")
        {
            ColladaLoader colladaLoader(file);

            if (colladaLoader.Load())
            {
                return colladaLoader.GenerateSceneGraph();
            }
            else
                return NodePtr();
        }
		else if (ext == "obj")
		{
			ObjLoader objLoader;

			objLoader.Load(file);
			return objLoader.GenerateSceneGraph();			
		}
    }
    return NodePtr();
}

vec3 Node::GetWorldPosition()
{
	return GetWorldTransformation() * vec3(0.0f, 0.0f, 0.0f);
}
