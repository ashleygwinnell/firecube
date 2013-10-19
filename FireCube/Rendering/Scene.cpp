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

Scene::Scene() : ambientColor(0.1f)
{

}

Scene::~Scene()
{
	for (auto renderable : renderables)
	{
		renderable->SetScene(nullptr);
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

void Scene::UpdateBaseQueue()
{
	baseQueue.Clear();
	for (auto renderable : renderables)
	{
		if (camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
		{
			for (auto &renderablePart : renderable->GetRenderableParts())
			{
				if (!renderablePart.geometry->GetMaterial())
					continue;
				TechniquePtr technique = renderablePart.geometry->GetMaterial()->GetTechnique();
				if (!technique)
					continue;
				RenderJob newRenderJob;				
				newRenderJob.pass = technique->GetPass(BASE_PASS);
				if (newRenderJob.pass == nullptr)
					continue;
				newRenderJob.vertexShader = newRenderJob.pass->GenerateVertexShader(0).get();
				newRenderJob.fragmentShader = newRenderJob.pass->GenerateFragmentShader(0).get();
				newRenderJob.geometry = renderablePart.geometry;
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
		unsigned int shaderProperties = 0;

		if (lights[i]->GetLightType() == DIRECTIONAL)
			shaderProperties |= STP_DIRECTIONAL_LIGHT;
		else if (lights[i]->GetLightType() == POINT)
			shaderProperties |= STP_POINT_LIGHT;
		else if (lights[i]->GetLightType() == SPOT)
			shaderProperties |= STP_SPOT_LIGHT;


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
					if (!renderablePart.geometry->GetMaterial())
						continue;
					TechniquePtr technique = renderablePart.geometry->GetMaterial()->GetTechnique();
					if (!technique)
						continue;
					RenderJob newRenderJob;
					newRenderJob.pass = technique->GetPass(LIGHT_PASS);
					if (newRenderJob.pass == nullptr)
						continue;
					newRenderJob.vertexShader = newRenderJob.pass->GenerateVertexShader(shaderProperties).get();
					newRenderJob.fragmentShader = newRenderJob.pass->GenerateFragmentShader(shaderProperties).get();
					newRenderJob.geometry = renderablePart.geometry;
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
	renderer->ResetCachedShadersParameters();

	UpdateRenderables();
	UpdateBaseQueue();
	UpdateLightQueues();		

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);	
	glDepthMask(true);

	baseQueue.Sort();
	for (auto &renderJob : baseQueue.GetRenderJobs())
	{			
		ProgramPtr program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);
		program->SetUniform(PARAM_AMBIENT_COLOR, ambientColor);		
		renderer->UseCamera(camera.get());
		// View transformation
		program->SetUniform(PARAM_MODEL_MATRIX, renderJob.transformation);		
		// Normal matrix which equals the inverse transpose of the model matrix
		mat3 normalMatrix = renderJob.transformation.ToMat3();
		normalMatrix.Inverse();
		normalMatrix.Transpose();				
		program->SetUniform(PARAM_NORMAL_MATRIX, normalMatrix);

		// Set material properties if this geometry is different from the last one 

		renderer->UseMaterial(renderJob.geometry->GetMaterial().get());
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
			ProgramPtr program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);
			program->SetUniform(PARAM_AMBIENT_COLOR, ambientColor);
			renderer->UseCamera(camera.get());			
			// View transformation
			program->SetUniform(PARAM_MODEL_MATRIX, renderJob.transformation);			
			// Normal matrix which equals the inverse transpose of the model matrix
			mat3 normalMatrix = renderJob.transformation.ToMat3();
			normalMatrix.Inverse();
			normalMatrix.Transpose();				
			program->SetUniform(PARAM_NORMAL_MATRIX, normalMatrix);
			// Set material properties if this geometry is different from the last one 
			renderer->UseMaterial(renderJob.geometry->GetMaterial().get());
			renderer->UseLight(light);
			renderJob.geometry->Render();
		}
	}

	glDisable(GL_BLEND);
	glDepthFunc(GL_LESS);		
	glDepthMask(true);
}

void Scene::SetRootNodeAndCamera(NodePtr rootNode, CameraPtr camera)
{
	this->rootNode = rootNode;
	rootNode->SetScene(this);
	this->camera = camera;
}

void Scene::SetAmbientColor(vec3 color)
{
	ambientColor = color;
}

void Scene::IntersectRay(RayQuery &rayQuery)
{
	for (auto renderable : renderables)
	{
		if (renderable->GetQueryIntersection())
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
	debugRenderer->Render(camera.get());
}