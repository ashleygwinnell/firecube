#include <algorithm>

#include "Scene.h"
#include "Rendering/Renderable.h"
#include "Geometry/Geometry.h"
#include "Rendering/Shader.h"
#include "Scene/Camera.h"
#include "Rendering/Texture.h"
#include "Geometry/Material.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Technique.h"
#include "Rendering/ShaderTemplate.h"
#include "Rendering/DebugRenderer.h"

using namespace FireCube;

Scene::Scene(Engine *engine) : Object(engine), ambientColor(0.1f), fogEnabled(false), rootNode(engine)
{
	rootNode.SetScene(this);
}

Scene::~Scene()
{
	for (auto renderable : renderables)
	{
		renderable->SetScene(nullptr);
	}

	for (auto light : lights)
	{
		light->SetScene(nullptr);
	}
}

void Scene::AddRenderable(Renderable *renderable)
{
	renderables.push_back(renderable);
}

void Scene::RemoveRenderable(Renderable *renderable)
{
	for (auto i = renderables.begin(); i != renderables.end(); ++i)
	{
		if (*i == renderable)
		{
			renderables.erase(i);
			break;
		}
	}
}

void Scene::UpdateRenderables()
{
	for (auto renderable : renderables)
		renderable->UpdateRenderableParts();
}

void Scene::AddLight(Light *light)
{
	lights.push_back(light);
}

void Scene::RemoveLight(Light *light)
{
	for (auto i = lights.begin(); i != lights.end(); ++i)
	{
		if (*i == light)
		{
			lights.erase(i);
			break;
		}
	}
}

void Scene::UpdateBaseQueue()
{
	baseQueue.Clear();
	for (auto renderable : renderables)
	{
		if (camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
		{
			for (auto &renderablePart : renderable->GetRenderableParts())
			{
				if (!renderablePart.material)
					continue;
				Technique *technique = renderablePart.material->GetTechnique();
				if (!technique)
					continue;
				RenderJob newRenderJob;				
				newRenderJob.pass = technique->GetPass(BASE_PASS);
				if (newRenderJob.pass == nullptr)
					continue;
				newRenderJob.pass->GenerateAllShaderPermutations();
				unsigned int shaderPermutation = 0;
				if (fogEnabled)
					shaderPermutation += 1;
				newRenderJob.vertexShader = newRenderJob.pass->GetGeneratedVertexShader(shaderPermutation);
				newRenderJob.fragmentShader = newRenderJob.pass->GetGeneratedFragmentShader(shaderPermutation);
				newRenderJob.geometry = renderablePart.geometry;
				newRenderJob.material = renderablePart.material;
				newRenderJob.transformation = renderablePart.transformation;
				newRenderJob.distance = (renderable->GetWorldBoundingBox().GetCenter() - camera->GetPosition()).Length();
				newRenderJob.CalculateSortKey();
				baseQueue.renderJobs.push_back(newRenderJob);
			}			
		}
	}
}

void Scene::UpdateLightQueues()
{
	lightQueues.resize(lights.size());
	for (unsigned int i = 0; i < lightQueues.size(); ++i)
	{
		unsigned int shaderPermutation = 0;

		if (lights[i]->GetLightType() == DIRECTIONAL)
			shaderPermutation = SP_DIRECTIONAL_LIGHT;
		else if (lights[i]->GetLightType() == POINT)
			shaderPermutation = SP_POINT_LIGHT;
		else if (lights[i]->GetLightType() == SPOT)
			shaderPermutation = SP_SPOT_LIGHT;
		if (fogEnabled)
			shaderPermutation += MAX_SHADER_PERMUTATIONS;

		lightQueues[i].light = lights[i];
		lightQueues[i].renderQueue.Clear();
		for (auto renderable : renderables)
		{
			if (camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
			{
				for (auto &renderablePart : renderable->GetRenderableParts())
				{
					// TODO: Need to cull spot lights using frustum
					if (lights[i]->GetLightType() == POINT || lights[i]->GetLightType() == SPOT)
					{ 
						BoundingBox bbox = renderable->GetWorldBoundingBox();
						float l = bbox.GetSize().Length();
						if ((bbox.GetCenter() - lights[i]->GetNode()->GetWorldPosition()).Length() - l> lights[i]->GetRange())
							continue;
					}
					if (!renderablePart.material)
						continue;
					Technique *technique = renderablePart.material->GetTechnique();
					if (!technique)
						continue;
					RenderJob newRenderJob;
					newRenderJob.pass = technique->GetPass(LIGHT_PASS);
					if (newRenderJob.pass == nullptr)
						continue;
					newRenderJob.pass->GenerateAllShaderPermutations();
					newRenderJob.vertexShader = newRenderJob.pass->GetGeneratedVertexShader(shaderPermutation);
					newRenderJob.fragmentShader = newRenderJob.pass->GetGeneratedFragmentShader(shaderPermutation);
					newRenderJob.geometry = renderablePart.geometry;
					newRenderJob.material = renderablePart.material;
					newRenderJob.transformation = renderablePart.transformation;
					newRenderJob.distance = (renderable->GetWorldBoundingBox().GetCenter() - camera->GetPosition()).Length();
					newRenderJob.CalculateSortKey();
					lightQueues[i].renderQueue.renderJobs.push_back(newRenderJob);
				}			
			}
		}
	}	
}

void Scene::Render(Renderer *renderer)
{
	renderer->ResetCachedShaderParameters();

	UpdateRenderables();
	UpdateBaseQueue();
	UpdateLightQueues();		

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);	
	glDepthMask(true);

	baseQueue.Sort();
	for (auto &renderJob : baseQueue.GetRenderJobs())
	{			
		Program *program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);
		program->SetUniform(PARAM_AMBIENT_COLOR, ambientColor);		
		if (fogEnabled)
		{
			program->SetUniform(PARAM_FOG_PARAMETERS, fogParameters);
			program->SetUniform(PARAM_FOG_COLOR, fogColor);
		}
		renderer->UseCamera(camera);
		// View transformation
		program->SetUniform(PARAM_MODEL_MATRIX, renderJob.transformation);		
		// Normal matrix which equals the inverse transpose of the model matrix
		mat3 normalMatrix = renderJob.transformation.ToMat3();
		normalMatrix.Inverse();
		normalMatrix.Transpose();				
		program->SetUniform(PARAM_NORMAL_MATRIX, normalMatrix);

		// Set material properties if this geometry is different from the last one 

		renderer->UseMaterial(renderJob.material);
		renderJob.geometry->Render();
	}

	glEnable(GL_BLEND);
	glDepthFunc(GL_EQUAL);	
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);

	for (auto &lightQueue : lightQueues)
	{
		lightQueue.renderQueue.Sort();
		Light *light = lightQueue.light;
		for (auto &renderJob : lightQueue.renderQueue.GetRenderJobs())
		{
			Program *program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);
			program->SetUniform(PARAM_AMBIENT_COLOR, ambientColor);
			if (fogEnabled)
			{
				program->SetUniform(PARAM_FOG_PARAMETERS, fogParameters);
				program->SetUniform(PARAM_FOG_COLOR, fogColor);
			}
			renderer->UseCamera(camera);
			// View transformation
			program->SetUniform(PARAM_MODEL_MATRIX, renderJob.transformation);			
			// Normal matrix which equals the inverse transpose of the model matrix
			mat3 normalMatrix = renderJob.transformation.ToMat3();
			normalMatrix.Inverse();
			normalMatrix.Transpose();				
			program->SetUniform(PARAM_NORMAL_MATRIX, normalMatrix);
			// Set material properties if this geometry is different from the last one 
			renderer->UseMaterial(renderJob.material);
			renderer->UseLight(light);
			renderJob.geometry->Render();
		}
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);		
	glDepthMask(true);
}

void Scene::SetCamera(Camera *camera)
{	
	this->camera = camera;
}

void Scene::SetAmbientColor(vec3 color)
{
	ambientColor = color;
}

void Scene::IntersectRay(RayQuery &rayQuery, unsigned int collisionQueryMask)
{
	for (auto renderable : renderables)
	{
		if (renderable->GetCollisionQueryMask() & collisionQueryMask)
			renderable->IntersectRay(rayQuery);
	}

	std::sort(rayQuery.results.begin(), rayQuery.results.end(),
		[](const RayQueryResult &a, const RayQueryResult &b)
	{ 
		return a.distance < b.distance; 
	});
}

void Scene::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	for (auto renderable : renderables)
		renderable->RenderDebugGeometry(debugRenderer);
	debugRenderer->Render(camera);
}

void Scene::SetFogEnabled(bool fogEnabled)
{
	this->fogEnabled = fogEnabled;
}

void Scene::SetFogParameters(vec3 fogParameters)
{
	this->fogParameters = fogParameters;
}

void Scene::SetFogColor(vec3 fogColor)
{
	this->fogColor = fogColor;
}

vec3 Scene::GetFogColor() const
{
	return fogColor;
}

Node *Scene::GetRootNode()
{
	return &rootNode;
}