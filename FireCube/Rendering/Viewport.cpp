#include "Viewport.h"
#include "Rendering/Renderable.h"
#include "Geometry/Geometry.h"
#include "Rendering/Shader.h"
#include "Scene/Camera.h"
#include "Rendering/Texture.h"
#include "Geometry/Material.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Technique.h"
#include "Rendering/ShaderTemplate.h"

using namespace FireCube;

Viewport::Viewport() : ambientColor(0.1f)
{

}

void Viewport::AddRenderable(Renderable *renderable)
{
	renderables.push_back(renderable);
}

void Viewport::UpdateRenderables()
{
	for (auto renderable : renderables)
		renderable->UpdateRenderableParts();
}

void Viewport::AddLight(Light *light)
{
	lights.push_back(light);
}

void Viewport::UpdateBaseQueue()
{
	baseQueue.Clear();
	for (auto renderable : renderables)
	{
		if (camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
		{
			for (auto &renderablePart : renderable->GetRenderableParts())
			{
				RenderJob newRenderJob;
				TechniquePtr technique = renderablePart.geometry->GetMaterial()->GetTechnique();
				if (!technique)
					continue;
				newRenderJob.pass = technique->GetPass(BASE_PASS);
				newRenderJob.vertexShader = newRenderJob.pass->GenerateVertexShader(0).get();
				newRenderJob.fragmentShader = newRenderJob.pass->GenerateFragmentShader(0).get();
				newRenderJob.geometry = renderablePart.geometry;
				newRenderJob.transformation = renderablePart.transformation;
				newRenderJob.CalculateSortKey();
				baseQueue.renderJobs.push_back(newRenderJob);
			}			
		}
	}
}

void Viewport::UpdateLightQueues()
{
	lightQueues.resize(lights.size());
	for (unsigned int i = 0; i < lightQueues.size(); ++i)
	{
		unsigned int shaderProperties = 0;

		if (lights[i]->GetLightType() == DIRECTIONAL)
			shaderProperties |= STP_DIRECTIONAL_LIGHT;
		else if (lights[i]->GetLightType() == POINT)
			shaderProperties |= STP_POINT_LIGHT;

		lightQueues[i].light = lights[i];
		lightQueues[i].renderQueue.Clear();
		for (auto renderable : renderables)
		{
			if (camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
			{
				for (auto &renderablePart : renderable->GetRenderableParts())
				{
					RenderJob newRenderJob;
					TechniquePtr technique = renderablePart.geometry->GetMaterial()->GetTechnique();
					if (!technique)
						continue;
					newRenderJob.pass = technique->GetPass(LIGHT_PASS);
					newRenderJob.vertexShader = newRenderJob.pass->GenerateVertexShader(shaderProperties).get();
					newRenderJob.fragmentShader = newRenderJob.pass->GenerateFragmentShader(shaderProperties).get();
					newRenderJob.geometry = renderablePart.geometry;
					newRenderJob.transformation = renderablePart.transformation;
					newRenderJob.CalculateSortKey();
					lightQueues[i].renderQueue.renderJobs.push_back(newRenderJob);
				}			
			}
		}
	}	
}

void Viewport::Render(Renderer *renderer)
{
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

void Viewport::SetRootNodeAndCamera(NodePtr rootNode, CameraPtr camera)
{
	this->rootNode = rootNode;
	rootNode->SetViewport(this);
	this->camera = camera;
}

void Viewport::SetAmbientColor(vec3 color)
{
	ambientColor = color;
}