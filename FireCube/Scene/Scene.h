#pragma once

#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Geometry/CollisionQuery.h"
#include "Core/Object.h"
#include "Rendering/RenderPath.h"
#include "Math/Octree.h"
#include "UI/UIElement.h"

namespace FireCube
{

class Renderable;
class DebugRenderer;

/**
* This class represents a scene. The scene contains the root node of a scene graph and
* also stores the renderables and lights in the graph.
*/
class Scene : public Object
{
	FIRECUBE_OBJECT(Scene)
	friend class Renderable;
	friend class Light;
	friend class Frame;
	friend class Node;
public:
	Scene(Engine *engine);
	~Scene();			
	
	/**
	* Sets the ambient color to use when rendering the scene
	* @param color The ambient color
	*/		
	void SetAmbientColor(vec3 color);
	
	/**
	* Intersects the ray with the rendrables in the scene
	* @param rayQuery The ray query which contains the ray itself and stores the result of the intersection test
	* @param collisionQueryMask The collision query mask which is used to exclude certain renderables from the test (those renderables whose collisionQueryMask AND'ed with this value results in 0)
	*/
	void IntersectRay(RayQuery &rayQuery, unsigned int collisionQueryMask = 0xFFFFFFFF);
	
	/**
	* Controls whether fog is enabled in the scene
	* @param fogEnabled Whether to enable fog
	*/
	void SetFogEnabled(bool fogEnabled);
	
	/**
	* Sets the fog parameters
	* @param fogParameters The linear fog parameters. x is the start z of the fog, y is the range of the fog. Both should be between 0.0 - 1.0 
	*/
	void SetFogParameters(vec3 fogParameters);
	
	/**
	* Sets the fog color
	* @param fogColor The fog color to use
	*/
	void SetFogColor(vec3 fogColor);
	
	/**
	* @returns The fog color
	*/
	vec3 GetFogColor() const;
	
	/**
	* @returns the root node of the scene graph
	*/
	Node *GetRootNode();	
	
	/**
	* @returns Whether fog is enabled
	*/
	bool GetFogEnabled() const;	
	
	/**
	* @returns The fog parameters
	*/
	vec3 GetFogParameters() const;
	
	/**
	* @returns The ambient color of the scene
	*/
	vec3 GetAmbientColor() const;

	UIElement *GetUI();
	
private:	

	/**
	* Adds a renderable to the scene
	* @param renderable The renderable to add
	*/
	void AddRenderable(Renderable *renderable);
	
	/**
	* Removes a renderable from the scene
	* @param renderable The renderable to remove
	*/
	void RemoveRenderable(Renderable *renderable);

	/**
	* Adds a light to the scene
	* @param light The light to add
	*/
	void AddLight(Light *light);

	/**
	* Removes a light from the scene
	* @param light The light to remove
	*/
	void RemoveLight(Light *light);

	void AddDelayedRemoveNode(Node *node);

	std::vector<Renderable *> &GetRenderables();
	std::vector<Light *> &GetLights();
	void UpdateRenderables();
	void PostRender(float deltaTime);

	Octree<Renderable> &GetOctree();
	
	Node rootNode;	
	std::vector<Renderable *> renderables;
	std::vector<Light *> lights;	
	vec3 ambientColor;
	bool fogEnabled;
	vec3 fogParameters;
	vec3 fogColor;		

	Octree<Renderable> octree;
	std::vector<SharedPtr<Node>> delayedRemoveNodes;
	UIElement ui;
};

}