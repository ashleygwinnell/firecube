#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "ThirdParty/GLEW/glew.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Texture.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/privateFont.h"
#include "Rendering/RenderPath.h"
#include "Rendering/ShaderTemplate.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Utils/Logger.h"
#include "Rendering/RenderSurface.h"

using namespace FireCube;

struct FontVertex
{
	vec3 position;
	vec2 uv;
};

Renderer::Renderer(Engine *engine) : Object(engine), textVao(0), numberOfPrimitivesRendered(0), 
	currentVertexShader(nullptr), currentFragmentShader(nullptr), currentMaterial(nullptr), currentCamera(nullptr),
	currentLight(nullptr), textVertexBuffer(nullptr), textVertexShader(nullptr), textFragmentShader(nullptr),
	textVertexShaderTemplate(nullptr), textFragmentShaderTemplate(nullptr), depthSurface(nullptr), currentFrameBuffer(nullptr), fboDirty(false)
{
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		renderTargets[i] = nullptr;
}

void Renderer::Initialize()
{
	// Create texture samplers for each texture unit (hard coded to 16)
	glGenSamplers(16, textureSampler);
	for (int i = 0; i < 16; i++)
	{
		glSamplerParameteri(textureSampler[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(textureSampler[i], GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// Create a vertex buffer for text rendering	
	glGenVertexArrays(1, &textVao);
	glBindVertexArray(textVao);
	textVertexBuffer = new VertexBuffer(this);
	glBindVertexArray(0);

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

	// Create shaders for text rendering
	textVertexShaderTemplate = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/font.vert");
	textFragmentShaderTemplate = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/font.frag");
	textVertexShader = textVertexShaderTemplate->GenerateShader("");
	textFragmentShader = textFragmentShaderTemplate->GenerateShader("");

	currentRenderPath = engine->GetResourceCache()->GetResource<RenderPath>("RenderPaths/Forward.xml");

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	blendMode = BlendMode::REPLACE;
	depthWrite = true;
}

void Renderer::Destroy()
{
	delete textVertexBuffer;
	delete quadVertexBuffer;

	glDeleteVertexArrays(1, &textVao);
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
	glBindTexture(GL_TEXTURE_2D, texture->GetObjectId());
	
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
	
	glBindSampler(unit, textureSampler[unit]);
}

void Renderer::RenderText(FontFace *fontFace, mat4 projectionMatrix, const vec3 &pos, const vec4 &color, const std::string &str)
{	
	if (!fontFace || str.empty())
		return;
	static std::vector<FontVertex> vBuffer;    
	vBuffer.resize(str.size() * 6);    
	Program *textProgram = SetShaders(textVertexShader, textFragmentShader);
	if (textProgram->IsValid())
	{
		UseProgram(textProgram);
		textProgram->SetUniform("tex0", 0);
		textProgram->SetUniform("textColor", color);
		textProgram->SetUniform("modelViewMatrix", mat4::IDENTITY);
		textProgram->SetUniform("projectionMatrix", projectionMatrix);
	}
	int numTris = 0;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	UseTexture(0, fontFace->page->tex);
	vec3 curPos = pos;
	FT_Long useKerning = FT_HAS_KERNING(fontFace->fontImpl->face);
	FT_UInt previous = 0;
	for (std::string::const_iterator i = str.begin(); i != str.end(); i++)
	{
		char c = *i;
		if (c == 32)
		{
			// If current glyph is space simply advance the current position
			curPos.x += fontFace->glyph[c].advance;
			continue;
		}
		else if (c == '\n')
		{
			// If current glyph is new line set the current position accordingly
			curPos.x = pos.x;
			curPos.y += fontFace->pointSize;
			continue;
		}
		else if (fontFace->glyph[c].size != vec2(0, 0))
		{

			FT_UInt glyphIndex = FT_Get_Char_Index( fontFace->fontImpl->face, c );
			// Retrieve kerning distance and move pen position
			if ( useKerning && previous && glyphIndex )
			{
				FT_Vector delta;
				FT_Get_Kerning( fontFace->fontImpl->face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta );
				curPos.x += delta.x >> 6;
			}
			// Populate the vertex buffer with the position and the texture coordinates of the current glyph
			vBuffer[numTris * 3 + 0].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
			vBuffer[numTris * 3 + 1].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, curPos.z);
			vBuffer[numTris * 3 + 2].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
			vBuffer[numTris * 3 + 0].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y);
			vBuffer[numTris * 3 + 1].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			vBuffer[numTris * 3 + 2].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y);
			numTris++;
			
			vBuffer[numTris * 3 + 0].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
			vBuffer[numTris * 3 + 1].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, curPos.z);			
			vBuffer[numTris * 3 + 2].position = vec3(fontFace->glyph[c].bitmapOffset.x + curPos.x + fontFace->glyph[c].size.x, fontFace->glyph[c].bitmapOffset.y + curPos.y + fontFace->glyph[c].size.y, curPos.z);
			vBuffer[numTris * 3 + 0].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y);
			vBuffer[numTris * 3 + 1].uv = vec2(fontFace->glyph[c].uv.x, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			vBuffer[numTris * 3 + 2].uv = vec2(fontFace->glyph[c].uv.x + fontFace->glyph[c].size.x / 512.0f, fontFace->glyph[c].uv.y + fontFace->glyph[c].size.y / 512.0f);
			numTris++;

			curPos.x += fontFace->glyph[c].advance;
			previous = glyphIndex;
		}
	}		
	textVertexBuffer->LoadData(&vBuffer[0], numTris * 3, VertexAttributeType::POSITION | VertexAttributeType::TEXCOORD0, BufferType::STREAM);
	glBindVertexArray(textVao);
	textVertexBuffer->ApplyAttributes();
	RenderStream(PrimitiveType::TRIANGLES, numTris * 3);
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
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

void Renderer::RenderStream(const PrimitiveType &primitiveType, unsigned int count)
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
	glDrawArrays(glmode, 0, count);
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
}

void Renderer::ResetNumberOfPrimitivesRendered()
{
	numberOfPrimitivesRendered = 0;
}

void Renderer::UseFrameBuffer(FrameBuffer *frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->GetObjectId());
	glViewport(0, 0, frameBuffer->width, frameBuffer->height);
	bool found = false;
	for (int i = 0; (i < MAX_TEXTURES) && (!found); i++)
		if ((frameBuffer->texture[i]) && (frameBuffer->texture[i]->IsValid()))
			found = true;
	if (found)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_NONE);
}

void Renderer::RestoreFrameBuffer()
{
	for (int i = 0; i < MAX_RENDER_TARGETS; ++i)
		SetRenderTarget(i, nullptr);

	depthSurface = nullptr;
	UpdateFrameBuffer();	
	SetViewport(0, 0, width, height);
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
	mat4 lightMatrix = mat4::IDENTITY;
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
		currentProgram->SetUniform(PARAM_LIGHT_SPOT_DIR, vec4(vec3(0, 0, 1).TransformNormal(light->GetNode()->GetWorldTransformation()), light->GetSpotCutOff()));		
	}
	currentProgram->SetUniform(PARAM_LIGHT_MATRIX, lightMatrix);
	currentProgram->SetUniform(PARAM_LIGHT_COLOR, light->GetColor());	
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
			hasColorBuffer = true;
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

void Renderer::SetCurrentRenderPath(RenderPath *renderPath)
{
	currentRenderPath = renderPath;
}

RenderPath *Renderer::GetCurrentRenderPath()
{
	return currentRenderPath;
}

void Renderer::RenderFullscreenQuad()
{
	RestoreFrameBuffer();
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

SharedPtr<RenderSurface> Renderer::GetRenderSurface(int width, int height, RenderSurfaceType type)
{	
	long long int key = ((long long) type << 32) | width << 16 | height;
	auto i = renderSurfaces.find(key);
	if (i != renderSurfaces.end())
		return i->second;

	SharedPtr<RenderSurface> renderSurface(new RenderSurface(this));
	if (type == RenderSurfaceType::COLOR)
	{
		Texture *texture = new Texture(engine);
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFiltering(TextureFilter::MIPMAP);
		UseTexture(0, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		
		renderSurface->SetLinkedTexture(texture);		
	}
	else if (type == RenderSurfaceType::DEPTH)
	{
		renderSurface->CreateDepth(width, height);
	}
	else if (type == RenderSurfaceType::DEPTH_TEXTURE)
	{
		Texture *texture = new Texture(engine);
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetFiltering(TextureFilter::LINEAR);
		UseTexture(0, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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