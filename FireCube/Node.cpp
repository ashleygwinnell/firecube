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
#include "GLee.h"

#include "utils.h"
#include "Logger.h"
#include "Filesystem.h"
#include "MyMath.h"
#include "BoundingBox.h"
#include "Geometry.h"
#include "Renderer.h"
#include "Light.h"
#include "Node.h"
#include "tinyxml.h"
#include "ModelLoaders.h"

using namespace FireCube;
void Light::SetType(LightType type)
{
    this->type = type;
}
LightType Light::GetType()
{
    return type;
}
void Light::SetAmbientColor(vec4 color)
{
    ambientColor = color;
}
vec4 Light::GetAmbientColor()
{
    return ambientColor;
}
void Light::SetDiffuseColor(vec4 color)
{
    diffuseColor = color;
}
vec4 Light::GetDiffuseColor()
{
    return diffuseColor;
}
void Light::SetSpecularColor(vec4 color)
{
    specularColor = color;
}
vec4 Light::GetSpecularColor()
{
    return specularColor;
}
bool Light::operator == (const Light &other) const
{
    return ambientColor == other.ambientColor && diffuseColor == other.diffuseColor && specularColor == other.specularColor && type == other.type;
}
NodeResource::NodeResource()
{
    rotation.Set(0, 0, 0);
    translation.Set(0, 0, 0);
    scale.Set(1, 1, 1);
    renderParameters.lighting = true;
    renderParameters.fog = false;
}
NodeResource::NodeResource(const string &name)
{
    rotation.Set(0, 0, 0);
    translation.Set(0, 0, 0);
    scale.Set(1, 1, 1);
    renderParameters.lighting = true;
    renderParameters.fog = false;
    SetName(name);
}
void NodeResource::SetName(const string &name)
{
    this->name = name;
}
string NodeResource::GetName()
{
    return name;
}
Node &NodeResource::GetParent()
{
    return parent;
}
mat4 NodeResource::GetLocalTransformation()
{
    if (!transformationChanged)
        return transformation;
    transformationChanged = false;
    transformation.Identity();
    transformation.Translate(translation);
    transformation.Scale(scale.x, scale.y, scale.z);
    transformation.RotateX(rotation.x);
    transformation.RotateY(rotation.y);
    transformation.RotateZ(rotation.z);
    transformation *= matTransform;
    return transformation;
}
mat4 NodeResource::GetWorldTransformation()
{
    return worldTransformation;
}
void NodeResource::SetWorldTransformation(mat4 mat)
{
    worldTransformation = mat;
}

void NodeResource::SetTranslation(vec3 t)
{
    transformationChanged = true;
    translation = t;
}
vec3 NodeResource::GetTranslation()
{
    return translation;
}
void NodeResource::SetRotation(vec3 r)
{
    transformationChanged = true;
    rotation = r;
}
vec3 NodeResource::GetRotation()
{
    return rotation;
}
void NodeResource::SetScale(vec3 s)
{
    transformationChanged = true;
    scale = s;
}
vec3 NodeResource::GetScale()
{
    return scale;
}
void NodeResource::SetMatrixTransformation(mat4 t)
{
    transformationChanged = true;
    matTransform = t;
}
mat4 NodeResource::GetMatrixTransformation()
{
    return matTransform;
}
void NodeResource::Move(vec3 t)
{
    transformationChanged = true;
    translation += t;
}
void NodeResource::Rotate(vec3 r)
{
    transformationChanged = true;
    rotation += r;
}
void NodeResource::Scale(vec3 s)
{
    transformationChanged = true;
    scale.x *= s.x;
    scale.y *= s.y;
    scale.z *= s.z;
}
void NodeResource::AddGeometry(Geometry geometry)
{
    geometries.push_back(geometry);
}
void NodeResource::AddLight(Light light)
{
    lights.push_back(light);
}
void NodeResource::RemoveLight(Light light)
{
    vector<Light>::iterator i = std::find(lights.begin(), lights.end(), light);
    if (i != lights.end())
        lights.erase(i);
}
void NodeResource::RenderBoundingBox()
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

    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->RenderBoundingBox();
}
void NodeResource::UpdateBoundingBox()
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

vector<Geometry> &NodeResource::GetGeometries()
{
    return geometries;
}
vector<Light> &NodeResource::GetLights()
{
    return lights;
}
void NodeResource::Render()
{
    if (parent)
        SetWorldTransformation(GetParent()->GetWorldTransformation()*GetLocalTransformation());
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
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
        (*i)->Render();
}
Node NodeResource::AddChild(Node node)
{
    children.push_back(node);
    if (node->GetParent())
        node->GetParent()->RemoveChild(node);
    node->parent = shared_from_this();
    return node;
}
void NodeResource::SetParent(Node parent)
{
    if (GetParent())
        GetParent()->RemoveChild(shared_from_this());
    this->parent = parent;
    if (parent)
        parent->children.push_back(shared_from_this());
}
Node NodeResource::GetChild(const string &name)
{
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i)->GetName() == name)
            return *i;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        Node ret = (*i)->GetChild(name);
        if (ret)
            return ret;
    }
    return Node();
}
Node NodeResource::RemoveChild(Node node)
{
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i) == node)
        {
            children.erase(i);
            return node;
        }
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        Node ret = (*i)->RemoveChild(node);
        if (ret)
            return ret;
    }
    return Node();
}
Node NodeResource::RemoveChild(const string &name)
{
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
        if ((*i)->GetName() == name)
        {
            Node ret = *i;
            children.erase(i);
            return ret;
        }
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        Node ret = (*i)->RemoveChild(name);
        if (ret)
            return ret;
    }
    return Node();
}
vector<Node> &NodeResource::GetChildren()
{
    return children;
}
void NodeResource::CreateHardNormals()
{
    for (vector<Geometry>::iterator i = GetGeometries().begin(); i != GetGeometries().end(); i++)
        (*i)->CreateHardNormals();
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->CreateHardNormals();
    }
}
RenderParameters &NodeResource::GetRenderParameters()
{
    return renderParameters;
}
void NodeResource::SetProgram(Program program)
{
    renderParameters.program = program;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetProgram(program);
    }
}
Program NodeResource::GetProgram()
{
    return renderParameters.program;
}
void NodeResource::SetTechnique(const string &name)
{
    renderParameters.technique = Renderer::GetTechnique(name);
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetTechnique(name);
    }
}
Technique NodeResource::GetTechnique() const
{
    return renderParameters.technique;
}
void NodeResource::SetLighting(bool enabled)
{
    renderParameters.lighting = enabled;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetLighting(enabled);
    }
}
bool NodeResource::GetLighting()
{
    return renderParameters.lighting;
}
void NodeResource::SetFog(bool enabled)
{
    renderParameters.fog = enabled;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetFog(enabled);
    }
}
bool NodeResource::GetFog()
{
    return renderParameters.fog;
}
void NodeResource::SetFogColor(vec4 color)
{
    renderParameters.fogColor = color;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetFogColor(color);
    }
}
vec4 NodeResource::GetFogColor()
{
    return renderParameters.fogColor;
}
void NodeResource::SetFogDensity(float density)
{
    renderParameters.fogDensity = density;
    for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
    {
        (*i)->SetFogDensity(density);
    }
}
float NodeResource::GetFogDensity()
{
    return renderParameters.fogDensity;
}
Node NodeResource::Clone()
{
	Node ret(new NodeResource);
	ret->geometries = geometries;
	ret->lights = lights;
	ret->localBoundingBox = localBoundingBox;
	ret->matTransform = matTransform;
	ret->name = name;
	ret->renderParameters = renderParameters;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->transformation = transformation;
	ret->transformationChanged = transformationChanged;
	ret->translation = translation;
	ret->worldBoundingBox = worldBoundingBox;
	ret->worldTransformation = worldTransformation;	
	for (vector<Node>::iterator i = children.begin(); i != children.end(); i++)
	{
		Node c=(*i)->Clone();
		c->SetParent(ret);
	}
	return ret;
}
Node FIRECUBE_API FireCube::LoadMesh(const string &filename)
{
    string file = Filesystem::SearchForFileName(filename);
    if (file.empty())
        return Node();
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
                return Node();
        }
        if (ext == "dae")
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