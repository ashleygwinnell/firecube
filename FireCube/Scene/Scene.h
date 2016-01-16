#pragma once

#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Geometry/CollisionQuery.h"
#include "Core/Object.h"
#include "Rendering/RenderPath.h"
#include "Math/Octree.h"

namespace FireCube
{

class Renderable;
class DebugRenderer;

class Scene : public Object
{
	FIRECUBE_OBJECT(Scene)
public:
	Scene(Engine *engine);
	~Scene();
	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);
	void AddLight(Light *light);
	void RemoveLight(Light *light);		
	void SetAmbientColor(vec3 color);
	void IntersectRay(RayQuery &rayQuery, unsigned int collisionQueryMask = 0xFFFFFFFF);
	void SetFogEnabled(bool fogEnabled);
	void SetFogParameters(vec3 fogParameters);
	void SetFogColor(vec3 fogColor);
	vec3 GetFogColor() const;
	Node *GetRootNode();	
	std::vector<Renderable *> &GetRenderables();
	std::vector<Light *> &GetLights();	
	void UpdateRenderables();
	bool GetFogEnabled() const;	
	vec3 GetFogParameters() const;
	vec3 GetAmbientColor() const;	
	Octree<Renderable> &GetOctree();
	
private:		
	
	Node rootNode;	
	std::vector<Renderable *> renderables;
	std::vector<Light *> lights;	
	vec3 ambientColor;
	bool fogEnabled;
	vec3 fogParameters;
	vec3 fogColor;		

	Octree<Renderable> octree;	
};

}