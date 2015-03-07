#pragma once

#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Geometry/CollisionQuery.h"
#include "Core/Object.h"
#include "Rendering/RenderPath.h"

namespace FireCube
{

class Renderable;
class DebugRenderer;

class Scene : public Object
{
	OBJECT(Scene)
public:
	Scene(Engine *engine);
	~Scene();
	void AddRenderable(Renderable *renderable);
	void RemoveRenderable(Renderable *renderable);
	void AddLight(Light *light);
	void RemoveLight(Light *light);
	void Render(Renderer *renderer);
	void RenderDebugGeometry(DebugRenderer *debugRenderer);
	void SetCamera(Camera *camera);
	void SetAmbientColor(vec3 color);
	void IntersectRay(RayQuery &rayQuery, unsigned int collisionQueryMask = 0xFFFFFFFF);
	void SetFogEnabled(bool fogEnabled);
	void SetFogParameters(vec3 fogParameters);
	void SetFogColor(vec3 fogColor);
	vec3 GetFogColor() const;
	Node *GetRootNode();
	void SetRenderTarget(SharedPtr<RenderSurface> renderSurface);
private:	
	void UpdateRenderables();
	void UpdateBaseQueue();
	void UpdateLightQueues();
	void SetRenderTargets(Renderer *renderer, const RenderPathCommand &command);
	void SetTextures(Renderer *renderer, const RenderPathCommand &command);
	void RenderDebugGeometry(DebugRenderer *debugRenderer, Node *node);
	void RenderShadowMap(Renderer *renderer, Light *light, RenderQueue &queue);

	Node rootNode;
	Camera *camera;
	std::vector<Renderable *> renderables;
	std::vector<Light *> lights;
	std::vector<std::pair<Light *, std::map<StringHash, RenderQueue>>> lightQueues;
	std::map<StringHash, RenderQueue> baseQueues;
	vec3 ambientColor;
	bool fogEnabled;
	vec3 fogParameters;
	vec3 fogColor;
	SharedPtr<RenderSurface> renderSurface;

};

}