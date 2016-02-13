#include <vector>
#include <map>

#include "ThirdParty/GLEW/glew.h"
#include "Geometry/Geometry.h"
#include "Rendering/Material.h"
#include "Rendering/Texture2D.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/RenderPath.h"
#include "Rendering/ShaderTemplate.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Utils/Logger.h"
#include "Rendering/RenderSurface.h"
#include "Rendering/Frame.h"

using namespace FireCube;

struct FontVertex
{
	vec3 position;
	vec2 uv;
};

Renderer::Renderer(Engine *engine) : Object(engine), numberOfPrimitivesRendered(0), 
	currentVertexShader(nullptr), currentFragmentShader(nullptr), currentMaterial(nullptr), currentCamera(nullptr),
	currentLight(nullptr), depthSurface(nullptr), currentFrameBuffer(nullptr), fboDirty(false)
{
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		renderTargets[i] = nullptr;
}

void Renderer::Initialize()
{
	// Create texture samplers for each texture unit (hard coded to 16)
	glGenSamplers(16, textureSampler);	

	glGenVertexArrays(1, &quadVao);
	glBindVertexArray(quadVao);

	quadVertexBuffer = new VertexBuffer(this);
	std::vector<float> quadVertices = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f };
	quadVertexBuffer->LoadData(&quadVertices[0], 6, VertexAttributeType::POSITION | VertexAttributeType::TEXCOORD0, BufferType::STATIC);
	quadVertexBuffer->ApplyAttributes();
	glBindVertexArray(0);

	glGenVertexArrays(1, &dummyVao);

	defaultRenderPath = engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/Forward.xml");

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	blendMode = BlendMode::REPLACE;
	depthWrite = true;

	shadowMap = GetRenderSurface(1024, 1024, RenderSurfaceType::DEPTH_TEXTURE);
}

void Renderer::Destroy()
{	
	delete quadVertexBuffer;	
	
	glDeleteVertexArrays(1, &quadVao);
	glDeleteSamplers(16, textureSampler);
}

void Renderer::Clear(const vec4 &color, float depth, ClearBufferType buffers)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClearDepth(depth);
	GLbitfield target = 0;
	if ((buffers & ClearBufferType::COLOR) == ClearBufferType::COLOR)
		target |= GL_COLOR_BUFFER_BIT;
	if ((buffers & ClearBufferType::DEPTH) == ClearBufferType::DEPTH)
		target |= GL_DEPTH_BUFFER_BIT;

	glClear(target);
}

void Renderer::UseTexture(unsigned int unit, const Texture *texture)
{
	if (!texture)
		return;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(texture->GetTarget(), texture->GetObjectId());
	
	GLint min = GL_LINEAR_MIPMAP_LINEAR, mag = GL_LINEAR;

	if (texture->filter == TextureFilter::NEAREST)
		min = GL_NEAREST;
	else if (texture->filter == TextureFilter::LINEAR)
		min = GL_LINEAR;
	else if (texture->filter == TextureFilter::MIPMAP)
		min = GL_LINEAR_MIPMAP_LINEAR;

	if (texture->filter == TextureFilter::NEAREST)
		mag = GL_NEAREST;
	else if (texture->filter == TextureFilter::LINEAR)
		mag = GL_LINEAR;
	else if (texture->filter == TextureFilter::MIPMAP)
		mag = GL_LINEAR;

	glSamplerParameteri(textureSampler[unit], GL_TEXTURE_MAG_FILTER, mag);
	glSamplerParameteri(textureSampler[unit], GL_TEXTURE_MIN_FILTER, min);
	
	GLint wrap;
	switch (texture->wrapMode)
	{
	case TextureWrapMode::CLAMP_TO_EDGE:
		wrap = GL_CLAMP_TO_EDGE;
		break;
	case TextureWrapMode::REPEAT:
		wrap = GL_REPEAT;
		break;
	default:
		break;
	}
	glSamplerParameteri(textureSampler[unit], GL_TEXTURE_WRAP_S, wrap);
	glSamplerParameteri(textureSampler[unit], GL_TEXTURE_WRAP_T, wrap);
	glSamplerParameteri(textureSampler[unit], GL_TEXTURE_WRAP_R, wrap);

	glBindSampler(unit, textureSampler[unit]);
}

void Renderer::RenderIndexStream(const PrimitiveType &primitiveType, unsigned int count)
{
	GLenum glmode;
	switch (primitiveType)
	{
	case PrimitiveType::POINTS:
		glmode = GL_POINTS;
		break;
	case PrimitiveType::LINES:
		glmode = GL_LINES;
		break;
	case PrimitiveType::TRIANGLES:
		glmode = GL_TRIANGLES;
		break;
	case PrimitiveType::TRIANGLE_STRIP:
		glmode = GL_TRIANGLE_STRIP;
		break;
	case PrimitiveType::QUADS:
		glmode = GL_QUADS;
		break;
	case PrimitiveType::LINE_LOOP:
		glmode = GL_LINE_LOOP;
		break;
	case PrimitiveType::LINE_STRIP:
		glmode = GL_LINE_STRIP;
		break;
	case PrimitiveType::TRIANGLE_FAN:
		glmode = GL_TRIANGLE_FAN;
		break;
	default:
		//TODO: Add log of error.
		return;
	}
	glDrawElements(glmode, count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::RenderStream(const PrimitiveType &primitiveType, unsigned int count, unsigned int offset)
{
	GLenum glmode;
	switch (primitiveType)
	{
	case PrimitiveType::POINTS:
		glmode = GL_POINTS;
		break;
	case PrimitiveType::LINES:
		glmode = GL_LINES;
		break;
	case PrimitiveType::TRIANGLES:
		glmode = GL_TRIANGLES;
		break;
	case PrimitiveType::TRIANGLE_STRIP:
		glmode = GL_TRIANGLE_STRIP;
		break;
	case PrimitiveType::QUADS:
		glmode = GL_QUADS;
		break;
	case PrimitiveType::LINE_LOOP:
		glmode = GL_LINE_LOOP;
		break;
	case PrimitiveType::LINE_STRIP:
		glmode = GL_LINE_STRIP;
		break;
	case PrimitiveType::TRIANGLE_FAN:
		glmode = GL_TRIANGLE_FAN;
		break;
	default:
		//TODO: Add log of error.
		return;
	}
	glDrawArrays(glmode, offset, count);
	glBindVertexArray(0);
}

void Renderer::UseProgram(Program *program)
{
	if (program)
		glUseProgram(program->GetObjectId());
}

void Renderer::UseMaterial(Material *material)
{
	if (!material)
		return;
	if (currentMaterial == material)
		return;

	currentMaterial = material;	
	const std::map<StringHash, Variant> &parameters = material->GetParameters();
	for (auto i = parameters.begin(); i != parameters.end(); ++i)
	{
		currentProgram->SetUniform(i->first, i->second);
	}

	Texture **textures = material->GetTextures();
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
	{
		TextureUnit textureUnit = (TextureUnit)i;
		if (textures[i])
			UseTexture(i, textures[i]);
	}

	switch (material->GetCullMode())
	{
	case CullMode::NONE:
		glDisable(GL_CULL_FACE);
		break;
	case CullMode::CW:
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		break;
	case CullMode::CCW:
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		break;
	default:
		break;
	}
}

void Renderer::ResetNumberOfPrimitivesRendered()
{
	numberOfPrimitivesRendered = 0;
}

void Renderer::UseFrameBuffer(FrameBuffer *frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetObjectId());
	glViewport(0, 0, frameBuffer->width, frameBuffer->height);	
}

void Renderer::RestoreFrameBuffer()
{
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		SetRenderTarget(i, nullptr);

	depthSurface = nullptr;
	UpdateFrameBuffer();		
}

void Renderer::SetViewport(int left, int right, int width, int height)
{
	glViewport(left, right, width, height);
}

Program *Renderer::SetShaders(Shader *vertexShader, Shader *fragmentShader)
{
	if (currentVertexShader == vertexShader && currentFragmentShader == fragmentShader)
		return currentProgram;

	currentVertexShader = vertexShader;
	currentFragmentShader = fragmentShader;
	
	ResetCachedShaderParameters();

	std::pair<Shader *, Shader *> key = std::make_pair(vertexShader, fragmentShader);
	auto i = programs.find(key);
	if (i != programs.end())
	{
		currentProgram = i->second;
	}
	else
	{
		currentProgram = new Program(this);
		currentProgram->Create(vertexShader, fragmentShader);
		programs[key] = currentProgram;
	}
	UseProgram(currentProgram);
	return currentProgram;
}

void Renderer::IncreamentNumberOfPrimitivesRendered(unsigned int amount)
{
	numberOfPrimitivesRendered += amount;
}

unsigned int Renderer::GetNumberOfPrimitivesRendered()
{
	return numberOfPrimitivesRendered;
}

void Renderer::UseCamera(Camera *camera)
{
	if (!camera)
		return;

	if (currentCamera == camera)
		return;

	currentCamera = camera;	
	// Projection transformation
	currentProgram->SetUniform(PARAM_VIEW_PROJECTION_MATRIX, camera->GetProjectionMatrix() * camera->GetViewMatrix());
	currentProgram->SetUniform(PARAM_CAMERA_POS, camera->GetNode()->GetWorldPosition());
}

void Renderer::UseLight(Light *light)
{
	if (!light)
		return;
	if (light == currentLight)
		return;

	currentLight = light;	
	if (light->GetLightType() == LightType::DIRECTIONAL)
	{
		currentProgram->SetUniform(PARAM_LIGHT_DIR, vec3(0, 0, 1).TransformNormal(light->GetNode()->GetWorldTransformation()));
	}
	else if (light->GetLightType() == LightType::POINT)
	{
		currentProgram->SetUniform(PARAM_LIGHT_POS, vec4(light->GetNode()->GetWorldTransformation().GetTranslation(), light->GetRange()));
	}	
	else if (light->GetLightType() == LightType::SPOT)
	{
		currentProgram->SetUniform(PARAM_LIGHT_POS, vec4(light->GetNode()->GetWorldTransformation().GetTranslation(), light->GetRange()));
		currentProgram->SetUniform(PARAM_LIGHT_SPOT_DIR, vec4(vec3(0, 0, 1).TransformNormal(light->GetNode()->GetWorldTransformation()), std::cos(light->GetSpotCutOff() * 0.5f)));
	}	
	currentProgram->SetUniform(PARAM_LIGHT_COLOR, light->GetColor());	

	if (light->GetCastShadow())
	{
		UseTexture(static_cast<int>(TextureUnit::SHADOWMAP), shadowMap->GetLinkedTexture());
		currentProgram->SetUniform(PARAM_SHADOW_INTENSITY, light->GetShadowIntensity());
	}
}

void Renderer::ResetCachedShaderParameters()
{
	currentMaterial = nullptr;
	currentCamera = nullptr;
	currentLight = nullptr;
}

void Renderer::SetRenderTarget(unsigned int index, RenderSurface *renderTarget)
{
	if (index >= MAX_RENDER_TARGETS)
		return;

	if (renderTargets[index] != renderTarget)
	{
		renderTargets[index] = renderTarget;
		fboDirty = true;
	}
}

void Renderer::SetDepthSurface(RenderSurface *depthSurface)
{
	if (this->depthSurface != depthSurface)
	{
		this->depthSurface = depthSurface;
		fboDirty = true;
	}
}

void Renderer::UpdateFrameBuffer()
{
	if (!fboDirty)
		return;

	fboDirty = false;
	bool hasFbo = (depthSurface != nullptr);
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		hasFbo = hasFbo | (renderTargets[i] != nullptr);

	if (!hasFbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		SetViewport(0, 0, width, height);
		return;
	}

	int width, height;

	if (renderTargets[0])
	{
		width = renderTargets[0]->GetWidth();
		height = renderTargets[0]->GetHeight();
	}
	else if (depthSurface)
	{
		width = depthSurface->GetWidth();
		height = depthSurface->GetHeight();
	}
	else
	{
		LOGERROR("Can't determine dimensions of render target");
		return;
	}

	unsigned int key = width << 16 | height;
	auto frameBufferIter = frameBuffers.find(key);
	FrameBuffer *frameBuffer;
	if (frameBufferIter != frameBuffers.end())
	{
		frameBuffer = frameBufferIter->second;
	}
	else
	{
		frameBuffer = new FrameBuffer(engine);
		frameBuffer->Create(width, height);		
		frameBuffers[key] = frameBuffer;
	}

	bool hasColorBuffer = false;
	
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		if (renderTargets[i])
		{
			frameBuffer->SetRenderTarget(renderTargets[i]->GetLinkedTexture(), i);
			if (renderTargets[i]->GetRenderSurfaceType() == RenderSurfaceType::COLOR)
			{
				hasColorBuffer = true;
			}			
		}
		else
		{
			frameBuffer->SetRenderTarget(nullptr, i);
		}
	}

	if (depthSurface) // TODO: Move this check to framebuffer object
	{		
		frameBuffer->SetDepthBufferSurface(depthSurface);
	}
	else
	{		
		frameBuffer->SetDepthBufferSurface(GetRenderSurface(width, height, RenderSurfaceType::DEPTH));		
	}

	if (hasColorBuffer)
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}
	else
	{
		glDrawBuffer(GL_NONE);
	}

	if (frameBuffer->IsValid() == false)
	{
		LOGERROR("Could not validate frame buffer.");
		RestoreFrameBuffer();
	}
	else
	{
		UseFrameBuffer(frameBuffer);
	}
}

RenderPath *Renderer::GetDefaultRenderPath()
{
	return defaultRenderPath;
}

void Renderer::RenderFullscreenQuad()
{	
	currentProgram->SetUniform(PARAM_VIEW_PROJECTION_MATRIX, mat4::IDENTITY);
	currentProgram->SetUniform(PARAM_CAMERA_POS, mat4::IDENTITY);	
	glBindVertexArray(quadVao);
	SetDepthTest(DepthTest::ALWAYS);
	SetBlendMode(BlendMode::REPLACE);
	RenderStream(PrimitiveType::TRIANGLES, 6);
}

void Renderer::SetWidth(int width)
{
	this->width = width;
}

void Renderer::SetHeight(int height)
{
	this->height = height;
}

int Renderer::GetWidth() const
{
	return width;
}

int Renderer::GetHeight() const
{
	return height;
}

SharedPtr<RenderSurface> Renderer::GetRenderSurface(int width, int height, RenderSurfaceType type, unsigned int uniqueKey)
{	
	long long int key = ((long long) type << 32) | width << 16 | height;
	key += ((long long)uniqueKey << 32);

	auto i = renderSurfaces.find(key);
	if (i != renderSurfaces.end())
		return i->second;

	SharedPtr<RenderSurface> renderSurface(new RenderSurface(this, type));
	if (type == RenderSurfaceType::COLOR)
	{
		Texture *texture = new Texture2D(engine);
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFiltering(TextureFilter::MIPMAP);
		UseTexture(0, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);		
		renderSurface->SetLinkedTexture(texture);		
	}
	else if (type == RenderSurfaceType::DEPTH)
	{
		renderSurface->CreateDepth(width, height);
	}
	else if (type == RenderSurfaceType::DEPTH_TEXTURE)
	{
		Texture *texture = new Texture2D(engine);
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFiltering(TextureFilter::NEAREST);
		UseTexture(0, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		texture->SetWrapMode(TextureWrapMode::CLAMP_TO_EDGE);
		renderSurface->SetLinkedTexture(texture);		
	}
	renderSurfaces[key] = renderSurface;
	return renderSurface;
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	if (this->blendMode == blendMode)
	{
		return;
	}

	this->blendMode = blendMode;

	switch (blendMode)
	{
	case BlendMode::REPLACE:
		glDisable(GL_BLEND);
		break;
	case BlendMode::ADD:
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		break;
	case BlendMode::ALPHA:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case BlendMode::ADDALPHA:
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	default:
		break;
	}
}

void Renderer::SetDepthWrite(bool depthWrite)
{
	if (this->depthWrite == depthWrite)
	{
		return;
	}

	this->depthWrite = depthWrite;
	glDepthMask(depthWrite);
}

void Renderer::SetDepthTest(DepthTest depthTest)
{
	if (this->depthTest == depthTest)
	{
		return;
	}

	this->depthTest = depthTest;
	switch (depthTest)
	{
	case DepthTest::ALWAYS:
		glDepthFunc(GL_ALWAYS);
		break;
	case DepthTest::NEVER:
		glDepthFunc(GL_NEVER);
		break;
	case DepthTest::EQUAL:
		glDepthFunc(GL_EQUAL);
		break;
	case DepthTest::LESSEQUAL:
		glDepthFunc(GL_LEQUAL);
		break;
	case DepthTest::LESS:
		glDepthFunc(GL_LESS);
		break;
	default:
		break;
	}
}

void Renderer::SetTimeStep(float timeStep)
{
	this->timeStep = timeStep;
}

float Renderer::GetTimeStep() const
{
	return timeStep;
}

RenderSurface *Renderer::GetShadowMap()
{
	return shadowMap;
}

void Renderer::SetBuffer(VertexBuffer *vertexBuffer)
{
	if (!vertexBuffer)
	{
		return;
	}

	glBindVertexArray(dummyVao);	
	vertexBuffer->ApplyAttributes();
}

void Renderer::SetSceneView(unsigned int index, SceneView *sceneView)
{
	if (index + 1 > sceneViews.size())
	{
		sceneViews.resize(index + 1);
	}

	sceneViews[index] = sceneView;
}

void Renderer::Render()
{
	for (auto i = sceneViews.rbegin(); i != sceneViews.rend(); ++i)
	{
		SceneView *sceneView = *i;
		Frame frame(engine, sceneView->GetScene(), sceneView->GetCamera(), sceneView->GetRenderSurface(), sceneView->GetRenderPath());
		frame.Render(this);
		
		RenderSurface *renderSurface = sceneView->GetRenderSurface();
		if (renderSurface && renderSurface->GetLinkedTexture())
		{
			renderSurface->GetLinkedTexture()->GenerateMipMaps();	
		}

	}
}