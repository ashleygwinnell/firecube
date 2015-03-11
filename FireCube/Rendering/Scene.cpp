#include <algorithm>

#include "Rendering/Renderable.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Technique.h"
#include "Rendering/ShaderTemplate.h"
#include "Rendering/DebugRenderer.h"
#include "Rendering/RenderPath.h"
#include "Rendering/RenderSurface.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Scene/Camera.h"
#include "Scene.h"
#include "Utils/Logger.h"
#include "Core/Engine.h"

using namespace FireCube;

Scene::Scene(Engine *engine) : Object(engine), ambientColor(0.1f), fogEnabled(false), rootNode(engine), fogColor(1.0f)
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
	baseQueues.clear();
	const std::vector<RenderPathCommand> commands = engine->GetRenderer()->GetCurrentRenderPath()->GetCommands();

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

				for (const auto &command : commands)
				{
					if (command.type == RenderPathCommandType::BASEPASS)
					{
						RenderQueue &queue = baseQueues[command.pass];
						RenderJob newRenderJob;
						newRenderJob.pass = technique->GetPass(command.pass);
						if (newRenderJob.pass == nullptr)
							continue;
						newRenderJob.pass->GenerateAllShaderPermutations();
						unsigned int vertexShaderPermutation = 0;
						unsigned int fragmentShaderPermutation = 0;
						if (renderablePart.geometry->GetGeometryType() == GeometryType::SKINNED)
							vertexShaderPermutation += 1;

						if (fogEnabled)
							fragmentShaderPermutation += 1;
						newRenderJob.vertexShader = newRenderJob.pass->GetGeneratedVertexShader(vertexShaderPermutation);
						newRenderJob.fragmentShader = newRenderJob.pass->GetGeneratedFragmentShader(fragmentShaderPermutation);
						newRenderJob.geometry = renderablePart.geometry;
						newRenderJob.material = renderablePart.material;
						newRenderJob.transformation = renderablePart.transformation;
						newRenderJob.distance = (renderable->GetWorldBoundingBox().GetCenter() - camera->GetNode()->GetWorldPosition()).Length();
						newRenderJob.skinMatrices = renderablePart.skinMatrices;
						newRenderJob.skinMatricesCount = renderablePart.skinMatricesCount;
						newRenderJob.CalculateSortKey();
						queue.renderJobs.push_back(newRenderJob);
					}
				}
			}
		}
	}
}

void Scene::UpdateLightQueues()
{
	const std::vector<RenderPathCommand> commands = engine->GetRenderer()->GetCurrentRenderPath()->GetCommands();

	lightQueues.clear();
	lightQueues.resize(lights.size());
	for (unsigned int i = 0; i < lightQueues.size(); ++i)
	{
		unsigned int vertexShaderPermutation = 0;
		unsigned int fragmentShaderPermutation = 0;

		if (lights[i]->GetLightType() == LightType::DIRECTIONAL)
		{
			vertexShaderPermutation = VSP_DIRECTIONAL_LIGHT;
			fragmentShaderPermutation = FSP_DIRECTIONAL_LIGHT;
		}
		else if (lights[i]->GetLightType() == LightType::POINT)
		{
			vertexShaderPermutation = VSP_POINT_LIGHT;
			fragmentShaderPermutation = FSP_POINT_LIGHT;
		}
		else if (lights[i]->GetLightType() == LightType::SPOT)
		{
			vertexShaderPermutation = VSP_SPOT_LIGHT;
			fragmentShaderPermutation = FSP_SPOT_LIGHT;
		}
		if (fogEnabled)
		{
			fragmentShaderPermutation += MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS;
		}

		lightQueues[i].first = lights[i];

		Camera *lightCamera = lights[i]->GetCamera();
		Node *lightCameraNode = lightCamera->GetNode();
		if (lights[i]->GetLightType() == LightType::DIRECTIONAL)
		{			
			lightCameraNode->SetTranslation(vec3(0.0f));
			lightCameraNode->SetRotation(lights[i]->GetNode()->GetWorldRotation());
		}
		else if (lights[i]->GetLightType() == LightType::SPOT)
		{
			lightCameraNode->SetTranslation(lights[i]->GetNode()->GetWorldTransformation().GetTranslation());
			lightCameraNode->SetRotation(lights[i]->GetNode()->GetWorldRotation());
		}

		BoundingBox visibleObjectsBoundingBox;

		for (auto renderable : renderables)
		{
			if ((renderable->GetLightMask() & lights[i]->GetLightMask()) && camera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
			{
				for (auto &renderablePart : renderable->GetRenderableParts())
				{
					// TODO: Need to cull spot lights using frustum
					if (lights[i]->GetLightType() == LightType::POINT || lights[i]->GetLightType() == LightType::SPOT)
					{
						BoundingBox bbox = renderable->GetWorldBoundingBox();
						float l = bbox.GetSize().Length();
						if ((bbox.GetCenter() - lights[i]->GetNode()->GetWorldPosition()).Length() - l > lights[i]->GetRange())
							continue;
					}
					if (!renderablePart.material)
						continue;
					Technique *technique = renderablePart.material->GetTechnique();
					if (!technique)
						continue;
					for (const auto &command : commands)
					{

						if (command.type == RenderPathCommandType::LIGHTPASS)
						{
							RenderQueue &queue = lightQueues[i].second[command.pass];
							RenderJob newRenderJob;
							newRenderJob.pass = technique->GetPass(command.pass);
							if (newRenderJob.pass == nullptr)
								continue;
							newRenderJob.pass->GenerateAllShaderPermutations();
							unsigned int currentPartVertexShaderPermutation = vertexShaderPermutation;
							unsigned int currentPartFragmentShaderPermutation = fragmentShaderPermutation;
							if (renderablePart.geometry->GetGeometryType() == GeometryType::SKINNED)
								currentPartVertexShaderPermutation += MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS;
							
							if (lights[i]->GetCastShadow() && (lights[i]->GetLightType() == LightType::DIRECTIONAL || lights[i]->GetLightType() == LightType::SPOT))
							{
								currentPartFragmentShaderPermutation += 3;
								currentPartVertexShaderPermutation += 3;
								if (renderable->GetCastShadow())
								{
									visibleObjectsBoundingBox.Expand(renderable->GetWorldBoundingBox());
								}
							}

							newRenderJob.vertexShader = newRenderJob.pass->GetGeneratedVertexShader(currentPartVertexShaderPermutation);
							newRenderJob.fragmentShader = newRenderJob.pass->GetGeneratedFragmentShader(currentPartFragmentShaderPermutation);
							newRenderJob.geometry = renderablePart.geometry;
							newRenderJob.material = renderablePart.material;
							newRenderJob.transformation = renderablePart.transformation;
							newRenderJob.distance = (renderable->GetWorldBoundingBox().GetCenter() - camera->GetNode()->GetWorldPosition()).Length();
							newRenderJob.skinMatrices = renderablePart.skinMatrices;
							newRenderJob.skinMatricesCount = renderablePart.skinMatricesCount;
							newRenderJob.CalculateSortKey();
							queue.renderJobs.push_back(newRenderJob);
						}
					}
				}
			}
		}
		
		if (lights[i]->GetCastShadow() && (lights[i]->GetLightType() == LightType::DIRECTIONAL || lights[i]->GetLightType() == LightType::SPOT))
		{
			mat4 lightView = lightCamera->GetViewMatrix();
			// Expand the visible bounding box a bit
			visibleObjectsBoundingBox.SetMin(visibleObjectsBoundingBox.GetMin() - vec3(1.0f));
			visibleObjectsBoundingBox.SetMax(visibleObjectsBoundingBox.GetMax() + vec3(1.0f));
			visibleObjectsBoundingBox.Transform(lightView);
			if (lights[i]->GetLightType() == LightType::DIRECTIONAL)
			{
				// Note: The z axis is flipped because the lights transformation determines the direction towards the light and not from the lights point of view
				lightCamera->SetOrthographicProjectionParameters(visibleObjectsBoundingBox.GetMin().x, visibleObjectsBoundingBox.GetMax().x, visibleObjectsBoundingBox.GetMin().y, visibleObjectsBoundingBox.GetMax().y, -visibleObjectsBoundingBox.GetMax().z, -visibleObjectsBoundingBox.GetMin().z);
			}
			else if (lights[i]->GetLightType() == LightType::SPOT)
			{
				lightCamera->SetPerspectiveProjectionParameters(lights[i]->GetSpotCutOff() * 180.0f / PI, 1.0f, 0.1f, lights[i]->GetRange());
			}

			for (auto renderable : renderables)
			{				
				if (renderable->GetCastShadow() && (renderable->GetLightMask() & lights[i]->GetLightMask()) && lightCamera->GetFrustum().Contains(renderable->GetWorldBoundingBox()))
				{
					for (auto &renderablePart : renderable->GetRenderableParts())
					{
						if (!renderablePart.material)
							continue;
						Technique *technique = renderablePart.material->GetTechnique();
						if (!technique)
							continue;

						Pass *pass = technique->GetPass(SHADOW_PASS);
						if (pass == nullptr)
							continue;

						RenderQueue &queue = lightQueues[i].second[SHADOW_PASS];
						RenderJob newRenderJob;
						newRenderJob.pass = pass;

						newRenderJob.pass->GenerateAllShaderPermutations();
						unsigned int vertexShaderPermutation = 0;
						unsigned int fragmentShaderPermutation = 0;
						if (renderablePart.geometry->GetGeometryType() == GeometryType::SKINNED)
							vertexShaderPermutation += 1;

						newRenderJob.vertexShader = newRenderJob.pass->GetGeneratedVertexShader(vertexShaderPermutation);
						newRenderJob.fragmentShader = newRenderJob.pass->GetGeneratedFragmentShader(fragmentShaderPermutation);
						newRenderJob.geometry = renderablePart.geometry;
						newRenderJob.material = renderablePart.material;
						newRenderJob.transformation = renderablePart.transformation;
						newRenderJob.distance = (renderable->GetWorldBoundingBox().GetCenter() - lightCameraNode->GetWorldPosition()).Length();
						newRenderJob.skinMatrices = renderablePart.skinMatrices;
						newRenderJob.skinMatricesCount = renderablePart.skinMatricesCount;
						newRenderJob.CalculateSortKey();
						queue.renderJobs.push_back(newRenderJob);
					}
				}
			}
		}
	}
}

void Scene::SetRenderTargets(Renderer *renderer, const RenderPathCommand &command)
{
	RenderSurface *renderTarget = command.renderPath->GetRenderTarget(command.output);
	if (!renderTarget)
		renderTarget = renderSurface;

	renderer->SetRenderTarget(0, renderTarget);		
	renderer->SetDepthSurface(nullptr);
	for (int i = 1; i < MAX_RENDER_TARGETS; ++i)
		renderer->SetRenderTarget(i, nullptr);
	renderer->UpdateFrameBuffer();	
}

void Scene::SetTextures(Renderer *renderer, const RenderPathCommand &command)
{
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
	{
		RenderSurface *renderTarget = command.renderPath->GetRenderTarget(command.textures[i]);
		if (renderTarget)
		{
			renderTarget->GetLinkedTexture()->GenerateMipMaps();
			renderer->UseTexture(i, renderTarget->GetLinkedTexture());
		}
	}
}

void Scene::Render(Renderer *renderer)
{
	renderer->ResetCachedShaderParameters();

	if (renderSurface)
		camera->SetAspectRatio((float)renderSurface->GetWidth() / (float)renderSurface->GetHeight());
	else
		camera->SetAspectRatio((float)renderer->GetWidth() / (float)renderer->GetHeight());

	UpdateRenderables();
	UpdateBaseQueue();
	UpdateLightQueues();		

	renderer->GetCurrentRenderPath()->AllocateRenderSurfaces();
	const std::vector<RenderPathCommand> commands = renderer->GetCurrentRenderPath()->GetCommands();

	for (auto &command : commands)
	{
		switch (command.type)
		{
		case RenderPathCommandType::CLEAR:
			SetRenderTargets(renderer, command);
			renderer->SetDepthWrite(true);
			if (command.useFogColor)
				renderer->Clear(vec4(fogColor, 1.0f), 1.0f, command.clearBuffers);
			else
				renderer->Clear(vec4(command.clearColor, 1.0f), 1.0f, command.clearBuffers);
			break;

		case RenderPathCommandType::BASEPASS:
		{
			SetRenderTargets(renderer, command);

			RenderQueue &queue = baseQueues[command.pass];
			queue.Sort();
			for (auto &renderJob : queue.GetRenderJobs())
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
				program->SetUniform(PARAM_TIME_STEP, renderer->GetTimeStep());
				// Set material properties if this geometry is different from the last one 
				renderer->UseMaterial(renderJob.material);
				renderer->SetBlendMode(renderJob.pass->GetBlendMode());
				renderer->SetDepthWrite(renderJob.pass->GetDepthWrite());
				renderer->SetDepthTest(renderJob.pass->GetDepthTest());								
				Geometry *geometry = renderJob.geometry;
				if (geometry->GetGeometryType() == GeometryType::SKINNED)
				{					
					program->SetUniform(PARAM_SKIN_MATRICES, renderJob.skinMatrices, renderJob.skinMatricesCount);
				}
				geometry->Render();
			}
			break;
		}
			

		case RenderPathCommandType::LIGHTPASS:
			SetRenderTargets(renderer, command);			

			for (auto &lightQueue : lightQueues)
			{
				RenderShadowMap(renderer, lightQueue.first, lightQueue.second[SHADOW_PASS]);
				SetRenderTargets(renderer, command);
				RenderQueue &queue = lightQueue.second[command.pass];
				queue.Sort();
				Light *light = lightQueue.first;
				for (auto &renderJob : queue.GetRenderJobs())
				{
					Program *program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);					
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
					program->SetUniform(PARAM_TIME_STEP, renderer->GetTimeStep());
					// Set material properties if this geometry is different from the last one 
					renderer->UseMaterial(renderJob.material);
					renderer->UseLight(light);
					renderer->SetBlendMode(renderJob.pass->GetBlendMode());
					renderer->SetDepthWrite(renderJob.pass->GetDepthWrite());
					renderer->SetDepthTest(renderJob.pass->GetDepthTest());		
										
					if (light->GetCastShadow() && (light->GetLightType() == LightType::DIRECTIONAL || light->GetLightType() == LightType::SPOT))
					{
						mat4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
										0.0f, 0.5f, 0.0f, 0.0f,
										0.0f, 0.0f, 0.5f, 0.0f,
										0.5f, 0.5f, 0.5f, 1.0f);						
						mat4 lightBiasMVPMatrix = biasMatrix * light->GetCamera()->GetProjectionMatrix() * light->GetCamera()->GetViewMatrix();
						program->SetUniform(PARAM_LIGHT_BIAS_MVP_MATRIX, lightBiasMVPMatrix);
					}
					Geometry *geometry = renderJob.geometry;
					if (geometry->GetGeometryType() == GeometryType::SKINNED)
					{
						program->SetUniform(PARAM_SKIN_MATRICES, renderJob.skinMatrices, renderJob.skinMatricesCount);
					}
					
					geometry->Render();									
				}				
			}
			
			break;

		case RenderPathCommandType::QUAD:
			SetRenderTargets(renderer, command);
			SetTextures(renderer, command);			
			renderer->SetShaders(command.vertexShader, command.fragmentShader);
			renderer->RenderFullscreenQuad();
			break;

		default:
			LOGERROR("Unkown command type: ", (int) command.type);
			continue;
		}
	}		
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
	RenderDebugGeometry(debugRenderer, &rootNode);
	debugRenderer->Render(camera);
}

void Scene::RenderDebugGeometry(DebugRenderer *debugRenderer, Node *node)
{
	auto &components = node->GetComponents();
	for (auto c : components)
	{
		c->RenderDebugGeometry(debugRenderer);
	}

	auto &children = node->GetChildren();

	for (auto child : children)
	{
		RenderDebugGeometry(debugRenderer, child);
	}
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

void Scene::SetRenderTarget(SharedPtr<RenderSurface> renderSurface)
{
	this->renderSurface = renderSurface;
}

void Scene::RenderShadowMap(Renderer *renderer, Light *light, RenderQueue &queue)
{
	RenderSurface *renderTarget = renderer->GetShadowMap();
	if (!renderTarget)
		return;

	renderer->SetDepthSurface(renderTarget);
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		renderer->SetRenderTarget(i, nullptr);
	renderer->UpdateFrameBuffer();
	
	Camera *lightCamera = light->GetCamera();	
	renderer->SetDepthWrite(true);
	renderer->SetDepthTest(DepthTest::LESS);
	renderer->SetBlendMode(BlendMode::REPLACE);
	renderer->Clear(vec4(0.0f), 1.0f, ClearBufferType::DEPTH);
	queue.Sort();
	for (auto &renderJob : queue.GetRenderJobs())
	{
		Program *program = renderer->SetShaders(renderJob.vertexShader, renderJob.fragmentShader);		
		renderer->UseCamera(lightCamera);
		// View transformation
		program->SetUniform(PARAM_MODEL_MATRIX, renderJob.transformation);		
		program->SetUniform(PARAM_TIME_STEP, renderer->GetTimeStep());
		// Set material properties if this geometry is different from the last one 
		renderer->UseMaterial(renderJob.material);
		renderer->SetBlendMode(renderJob.pass->GetBlendMode());
		renderer->SetDepthWrite(renderJob.pass->GetDepthWrite());
		renderer->SetDepthTest(renderJob.pass->GetDepthTest());
		Geometry *geometry = renderJob.geometry;
		if (geometry->GetGeometryType() == GeometryType::SKINNED)
		{
			program->SetUniform(PARAM_SKIN_MATRICES, renderJob.skinMatrices, renderJob.skinMatricesCount);
		}
		geometry->Render();
	}
}