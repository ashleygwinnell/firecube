#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/ResourceManager.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Texture.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shaders.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderQueue.h"
#include "Rendering/privateFont.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"

using namespace FireCube;
BufferPtr textVertexBuffer;
GLuint textVao = 0;
TextureManager *currentTextureManager = NULL;
ShaderManager *currentShaderManager = NULL;
FontManager *currentFontManager = NULL;
map<string, TechniquePtr> techniques;
CameraPtr camera;
unsigned int numberOfTrianglesRendered = 0;
GLuint textureSampler;

/** \cond */
struct FontVertex
{
	vec3 position;
	vec2 uv;
};
/** \endcond */
ShaderProperties::ShaderProperties() : directionalLighting(false), diffuseTexture(false), fog(false), pointLighting(false), normalTexture(false)
{

}
void ShaderProperties::FromMaterial(MaterialPtr mat)
{
    if (mat->GetDiffuseTexture())
        this->diffuseTexture = true;
    if (mat->GetNormalTexture())
        this->normalTexture = true;
}
void ShaderProperties::SetDirectionalLighting(bool enable)
{
    directionalLighting = enable;
}
void ShaderProperties::SetPointLighting(bool enable)
{
    pointLighting = enable;
}

void ShaderProperties::SetFog(bool enable)
{
    fog = enable;
}
unsigned int ShaderProperties::ToInt() const
{
    unsigned int ret = 0;
    if (directionalLighting)
        ret |= 1;
    if (fog)
        ret |= 2;
    if (diffuseTexture)
        ret |= 4;
    if (pointLighting)
        ret |= 8;
    if (normalTexture)
        ret |= 16;
    return ret;
}

void Renderer::Clear(const vec4 &color, float depth)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClearDepth(depth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::UseTexture(TexturePtr tex, unsigned int unit)
{
    if (!tex)
        return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);
	
	GLint min, mag;

	if (tex->minFilter == NEAREST)
		min = GL_NEAREST;
	else if (tex->minFilter == LINEAR)
		min = GL_LINEAR;
	else if (tex->minFilter == MIPMAP)
		min = GL_LINEAR_MIPMAP_LINEAR;

	if (tex->magFilter == NEAREST)
		mag = GL_NEAREST;
	else if (tex->magFilter == LINEAR)
		mag = GL_LINEAR;

	glSamplerParameteri(textureSampler, GL_TEXTURE_MAG_FILTER, mag);
	glSamplerParameteri(textureSampler, GL_TEXTURE_MIN_FILTER, min);
	
	glBindSampler(unit, textureSampler);
}
void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const string &str)
{	
    if (!font)
        return;
	if (str.empty())
		return;
    static vector<FontVertex> vBuffer;    
    vBuffer.resize(str.size() * 6);    
	ProgramPtr textProgram = Renderer::GetTechnique("font")->GenerateProgram(ShaderProperties());
    if (textProgram->IsValid())
    {
        UseProgram(textProgram);
        textProgram->SetUniform("tex0", 0);
        textProgram->SetUniform("textColor", color);
		textProgram->SetUniform("modelViewMatrix", Renderer::GetCamera()->GetViewMatrix());
		textProgram->SetUniform("projectionMatrix", Renderer::GetCamera()->GetProjectionMatrix());
    }
    int numTris = 0;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    UseTexture(font->page->tex, 0);
    vec3 curPos = pos;
    FT_Long useKerning = FT_HAS_KERNING(font->fontImpl->face);
    FT_UInt previous = 0;
    for (string::const_iterator i = str.begin(); i != str.end(); i++)
    {
        char c = *i;
        if (c == 32)
        {
            curPos.x += font->glyph[c].advance;
            continue;
        }
		if (c == '\n')
		{
			curPos.x = pos.x;
			curPos.y += font->size;
			continue;
		}
        if (font->glyph[c].size != vec2(0, 0))
        {

            FT_UInt glyphIndex = FT_Get_Char_Index( font->fontImpl->face, c );
            /* retrieve kerning distance and move pen position */
            if ( useKerning && previous && glyphIndex )
            {
                FT_Vector delta;
                FT_Get_Kerning( font->fontImpl->face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta );
                curPos.x += delta.x >> 6;
            }
			
            vBuffer[numTris * 3 + 0].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x, font->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
            vBuffer[numTris * 3 + 1].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x, font->glyph[c].bitmapOffset.y + curPos.y + font->glyph[c].size.y, curPos.z);
            vBuffer[numTris * 3 + 2].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x + font->glyph[c].size.x, font->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
			vBuffer[numTris * 3 + 0].uv = vec2(font->glyph[c].uv.x, font->glyph[c].uv.y);
			vBuffer[numTris * 3 + 1].uv = vec2(font->glyph[c].uv.x, font->glyph[c].uv.y + font->glyph[c].size.y / 512.0f);
			vBuffer[numTris * 3 + 2].uv = vec2(font->glyph[c].uv.x + font->glyph[c].size.x / 512.0f, font->glyph[c].uv.y);
			numTris++;
			
			vBuffer[numTris * 3 + 0].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x + font->glyph[c].size.x, font->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
			vBuffer[numTris * 3 + 1].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x, font->glyph[c].bitmapOffset.y + curPos.y + font->glyph[c].size.y, curPos.z);			
			vBuffer[numTris * 3 + 2].position = vec3(font->glyph[c].bitmapOffset.x + curPos.x + font->glyph[c].size.x, font->glyph[c].bitmapOffset.y + curPos.y + font->glyph[c].size.y, curPos.z);
			vBuffer[numTris * 3 + 0].uv = vec2(font->glyph[c].uv.x + font->glyph[c].size.x / 512.0f, font->glyph[c].uv.y);
			vBuffer[numTris * 3 + 1].uv = vec2(font->glyph[c].uv.x, font->glyph[c].uv.y + font->glyph[c].size.y / 512.0f);
			vBuffer[numTris * 3 + 2].uv = vec2(font->glyph[c].uv.x + font->glyph[c].size.x / 512.0f, font->glyph[c].uv.y + font->glyph[c].size.y / 512.0f);
			numTris++;

            curPos.x += font->glyph[c].advance;
            previous = glyphIndex;
        }
    }		
    textVertexBuffer->LoadData(&vBuffer[0], numTris * 3 * sizeof(FontVertex), STREAM);
	glBindVertexArray(textVao);
	RenderStream(TRIANGLES, numTris * 3);
	glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
void Renderer::RenderIndexStream(const PrimitiveType &primitiveType, unsigned int count)
{
    GLenum glmode;
    switch (primitiveType)
    {
    case POINTS:
        glmode = GL_POINTS;
        break;
    case LINES:
        glmode = GL_LINES;
        break;
    case TRIANGLES:
        glmode = GL_TRIANGLES;
        break;
    case TRIANGLE_STRIP:
        glmode = GL_TRIANGLE_STRIP;
        break;
    case QUADS:
        glmode = GL_QUADS;
        break;
    case LINE_LOOP:
        glmode = GL_LINE_LOOP;
        break;
	case LINE_STRIP:
		glmode = GL_LINE_STRIP;
		break;
    case TRIANGLE_FAN:
        glmode = GL_TRIANGLE_FAN;
        break;
    }
    glDrawElements(glmode, count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Renderer::RenderStream(const PrimitiveType &primitiveType, unsigned int count)
{
    GLenum glmode;
    switch (primitiveType)
    {
    case POINTS:
        glmode = GL_POINTS;
        break;
    case LINES:
        glmode = GL_LINES;
        break;
    case TRIANGLES:
        glmode = GL_TRIANGLES;
        break;
    case TRIANGLE_STRIP:
        glmode = GL_TRIANGLE_STRIP;
        break;
    case QUADS:
        glmode = GL_QUADS;
        break;
    case LINE_LOOP:
        glmode = GL_LINE_LOOP;
        break;
	case LINE_STRIP:
		glmode = GL_LINE_STRIP;
		break;
    case TRIANGLE_FAN:
        glmode = GL_TRIANGLE_FAN;
        break;
    }
    glDrawArrays(glmode, 0, count);
	glBindVertexArray(0);
}
void Renderer::UseProgram(ProgramPtr program)
{
    if (program)
        glUseProgram(program->GetId());
}
void Renderer::UseMaterial(ProgramPtr program, MaterialPtr material)
{
    if (!material)
        return;

    vec4 ambientColor = vec4(material->GetAmbientColor().x, material->GetAmbientColor().y, material->GetAmbientColor().z, 1.0f);
    vec4 diffuseColor = vec4(material->GetDiffuseColor().x, material->GetDiffuseColor().y, material->GetDiffuseColor().z, 1.0f);
    vec4 specularColor = vec4(material->GetSpecularColor().x, material->GetSpecularColor().y, material->GetSpecularColor().z, 1.0f);
    program->SetUniform("materialAmbient", ambientColor);
	program->SetUniform("materialDiffuse", diffuseColor);
	program->SetUniform("materialSpecular", specularColor);
	program->SetUniform("materialShininess", material->GetShininess());
	
    if ((material->GetDiffuseTexture()) && (material->GetDiffuseTexture()->IsValid()))
    {
        UseTexture(material->GetDiffuseTexture(), 0);
    }
    if ((material->GetNormalTexture()) && (material->GetNormalTexture()->IsValid()))
    {
        UseTexture(material->GetNormalTexture(), 1);
    }
}
void ResetNumberOfTrianglesRendered()
{
	numberOfTrianglesRendered = 0;
}
void InitializeRenderer()
{
	textVertexBuffer = BufferPtr(new Buffer);
	textVertexBuffer->Create();

    TechniquePtr technique(new Technique);
    if (!technique->LoadShader("default.vert") && !technique->LoadShader("Shaders/default.vert") && !technique->LoadShader("../Assets/Shaders/default.vert") && !technique->LoadShader("./Assets/Shaders/default.vert"))
        Logger::Write(Logger::LOG_WARNING, "Could not load default technique's vertex shader");
    else
    {
        if (!technique->LoadShader("default.frag") && !technique->LoadShader("Shaders/default.frag") && !technique->LoadShader("../Assets/Shaders/default.frag") && !technique->LoadShader("./Assets/Shaders/default.frag"))
            Logger::Write(Logger::LOG_WARNING, "Could not load default technique's fragment shader");
        else
            Renderer::AddTechnique("default", technique);
    }

	technique = TechniquePtr(new Technique);
	if (!technique->LoadShader("font.vert") && !technique->LoadShader("Shaders/font.vert") && !technique->LoadShader("../Assets/Shaders/font.vert") && !technique->LoadShader("./Assets/Shaders/font.vert"))
		Logger::Write(Logger::LOG_WARNING, "Could not load font technique's vertex shader");
	else
	{
		if (!technique->LoadShader("font.frag") && !technique->LoadShader("Shaders/font.frag") && !technique->LoadShader("../Assets/Shaders/font.frag") && !technique->LoadShader("./Assets/Shaders/font.frag"))
			Logger::Write(Logger::LOG_WARNING, "Could not load font technique's fragment shader");
		else
			Renderer::AddTechnique("font", technique);
	}

	Renderer::UseCamera(CameraPtr(new Camera));
		
	glGenSamplers(1, &textureSampler);	
	glSamplerParameteri(textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glGenVertexArrays(1, &textVao);
	glBindVertexArray(textVao);
	textVertexBuffer->SetVertexAttribute(0, 3, sizeof(FontVertex), 0);
	textVertexBuffer->SetVertexAttribute(4, 2, sizeof(FontVertex), sizeof(vec3));
	Renderer::DisableVertexAttribute(1);
	Renderer::DisableVertexAttribute(2);
	Renderer::DisableVertexAttribute(3);
	glBindVertexArray(0);
}
void Renderer::SetTextureManager(TextureManager &textureManager)
{
    currentTextureManager = &textureManager;
}
TextureManager &Renderer::GetTextureManager()
{
    return *currentTextureManager;
}
void Renderer::SetShaderManager(ShaderManager &shaderManager)
{
    currentShaderManager = &shaderManager;
}
ShaderManager &Renderer::GetShaderManager()
{
    return *currentShaderManager;
}
void Renderer::SetFontManager(FontManager &fontManager)
{
    currentFontManager = &fontManager;
}
FontManager &Renderer::GetFontManager()
{
    return *currentFontManager;
}
void DestroyRenderer()
{
	techniques.clear();
    textVertexBuffer = BufferPtr();
}
void Renderer::UseFrameBuffer(FrameBufferPtr frameBuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->id);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::SetViewport(int left, int right, int width, int height)
{
    glViewport(left, right, width, height);
}
void Renderer::DisableTexCoordStream(unsigned int unit)
{
    Renderer::DisableVertexAttribute(4);
}
void Renderer::DisableNormalStream()
{
    Renderer::DisableVertexAttribute(1);
}
void FIRECUBE_API Renderer::Render(NodePtr node)
{
	RenderQueue renderQueue;
	renderQueue.AddNode(node, camera);
	Renderer::Render(renderQueue);
}
void FIRECUBE_API Renderer::Render(NodePtr node, const string &techniqueName, const ProgramUniformsList &programUniformsList)
{
	RenderQueue renderQueue;
	renderQueue.AddNode(node);
	Renderer::Render(renderQueue, techniqueName, programUniformsList);
}
void FIRECUBE_API Renderer::Render(RenderQueue &renderQueue)
{
	int passNum = 0;
	vector<pair<mat4, Light>>::iterator i;
	vector<RenderJob>::iterator j;
	ProgramPtr lastProgram;
	GeometryPtr lastGeometry;
	glEnable(GL_BLEND);

	for (i = renderQueue.activeLights.begin(); i != renderQueue.activeLights.end(); i++)
	{
		for (j = renderQueue.renderJobs[RenderQueue::NORMAL].begin(); j != renderQueue.renderJobs[RenderQueue::NORMAL].end(); j++)
		{
			if (j->renderParameters.program && j->renderParameters.program->IsValid())
				j->program = j->renderParameters.program;
			else
			{
				ShaderProperties shaderProperties;
				shaderProperties.FromMaterial(j->geometry->GetMaterial());
				if (i->second.GetType() == DIRECTIONAL)
					shaderProperties.SetDirectionalLighting(true);
				else if (i->second.GetType() == POINT)
					shaderProperties.SetPointLighting(true);					
				shaderProperties.SetFog(j->renderParameters.fog);
				TechniquePtr technique = j->renderParameters.technique;
				if (technique)
					j->program = technique->GenerateProgram(shaderProperties);					
			}		
		}
		renderQueue.Sort(RenderQueue::NORMAL);
		if (passNum == 0)
		{
			glDepthFunc(GL_LESS);
			glBlendFunc(GL_ONE, GL_ZERO);
			glDepthMask(true);
		}
		else
		{
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_ONE, GL_ONE);
			glDepthMask(false);
		}
		for (j = renderQueue.renderJobs[RenderQueue::NORMAL].begin(); j != renderQueue.renderJobs[RenderQueue::NORMAL].end(); j++)
		{			
			ProgramPtr p;			
			p = j->program;
			if (!p)
				continue;						
				
			if (lastProgram != p)
			{
				lastProgram = p;
				Renderer::UseProgram(p);					
				if (i->second.GetType() == DIRECTIONAL)
				{
					p->SetUniform("directionalLightDir", vec3(0, 0, 1).TransformNormal(camera->GetViewMatrix() * i->first));
					p->SetUniform("lightAmbient", i->second.GetAmbientColor());
					p->SetUniform("lightDiffuse", i->second.GetDiffuseColor());
					p->SetUniform("lightSpecular", i->second.GetSpecularColor());
				}
				else if (i->second.GetType() == POINT)
				{
					p->SetUniform("lightPosition", vec3(0, 0, 0) * i->first * camera->GetViewMatrix());
					p->SetUniform("lightAmbient", i->second.GetAmbientColor());
					p->SetUniform("lightDiffuse", i->second.GetDiffuseColor());
					p->SetUniform("lightSpecular", i->second.GetSpecularColor());
				}													
			}
			if (j->geometry->GetMaterial()->GetDiffuseTexture())
				p->SetUniform("diffuseMap", 0);
			if (j->geometry->GetMaterial()->GetNormalTexture())
				p->SetUniform("normalMap", 1);
			p->SetUniform("modelViewMatrix", Renderer::GetCamera()->GetViewMatrix() * j->transformation);
			p->SetUniform("projectionMatrix", Renderer::GetCamera()->GetProjectionMatrix());
			mat3 normalMatrix = (Renderer::GetCamera()->GetViewMatrix()  * j->transformation).ToMat3();
			normalMatrix.Inverse();
			normalMatrix.Transpose();				
			p->SetUniform("normalMatrix", normalMatrix);
			if (j->renderParameters.fog)
			{
				if (passNum == 0)
				{
					p->SetUniform("fogColor", j->renderParameters.fogColor);						
				}
				else if (passNum == 1 || lastProgram != p)
				{
					p->SetUniform("fogColor", vec4(0, 0, 0, 0));
				}
				if (lastProgram != p)
					p->SetUniform("fogDensity", j->renderParameters.fogDensity);
			}
			if (lastGeometry != j->geometry)
			{
				lastGeometry = j->geometry;
				glBindVertexArray(j->geometry->vao);	
				Renderer::UseMaterial(p, j->geometry->GetMaterial());
			}
			
			if (j->geometry->indexBuffer && j->geometry->indexBuffer->IsValid())			
				Renderer::RenderIndexStream(j->geometry->GetPrimitiveType(), j->geometry->GetVertexCount());							
			else
				Renderer::RenderStream(j->geometry->GetPrimitiveType(), j->geometry->GetVertexCount());							
			
			numberOfTrianglesRendered += j->geometry->GetPrimitiveCount();			
		}
		passNum++;
	}
	glDepthMask(true);
	glDisable(GL_BLEND);

	for (j = renderQueue.renderJobs[RenderQueue::NON_LIGHTED].begin(); j != renderQueue.renderJobs[RenderQueue::NON_LIGHTED].end(); j++)
	{		
		if (j->renderParameters.program && j->renderParameters.program->IsValid())
			j->program = j->renderParameters.program;
		else
		{
			ShaderProperties shaderProperties;
			shaderProperties.FromMaterial(j->geometry->GetMaterial());
			shaderProperties.SetDirectionalLighting(false);
			shaderProperties.SetPointLighting(false);					
			shaderProperties.SetFog(j->renderParameters.fog);
			TechniquePtr technique = j->renderParameters.technique;
			if (technique)
				j->program = technique->GenerateProgram(shaderProperties);					
		}				
	}
	renderQueue.Sort(RenderQueue::NON_LIGHTED);
	for (j = renderQueue.renderJobs[RenderQueue::NON_LIGHTED].begin(); j != renderQueue.renderJobs[RenderQueue::NON_LIGHTED].end(); j++)
	{		
		ProgramPtr p;		
		p = j->program;
		if (!p)
			continue;		
			
		if (lastProgram != p)
		{
			lastProgram = p;
			Renderer::UseProgram(p);
			if (j->renderParameters.fog)
			{
				p->SetUniform("fogColor", j->renderParameters.fogColor);
				p->SetUniform("fogDensity", j->renderParameters.fogDensity);
			}			
		}	
		if (j->geometry->GetMaterial()->GetDiffuseTexture())
			p->SetUniform("diffuseMap", 0);
		if (j->geometry->GetMaterial()->GetNormalTexture())
			p->SetUniform("normalMap", 1);
		p->SetUniform("modelViewMatrix", Renderer::GetCamera()->GetViewMatrix() * j->transformation);
		p->SetUniform("projectionMatrix", Renderer::GetCamera()->GetProjectionMatrix());
		mat3 normalMatrix = (Renderer::GetCamera()->GetViewMatrix() * j->transformation).ToMat3();
		normalMatrix.Inverse();
		normalMatrix.Transpose();			
		p->SetUniform("normalMatrix", normalMatrix);
		if (lastGeometry != j->geometry)
		{
			lastGeometry = j->geometry;
			glBindVertexArray(j->geometry->vao);	
			Renderer::UseMaterial(p, j->geometry->GetMaterial());
		}			
		
		if (j->geometry->indexBuffer && j->geometry->indexBuffer->IsValid())
		{
			Renderer::RenderIndexStream(j->geometry->GetPrimitiveType(), j->geometry->GetVertexCount());			
			numberOfTrianglesRendered += j->geometry->GetPrimitiveCount();			
		}
		else
		{
			Renderer::RenderStream(j->geometry->GetPrimitiveType(), j->geometry->GetVertexCount());			
			numberOfTrianglesRendered += j->geometry->GetPrimitiveCount();			
		}
	}		
}
void FIRECUBE_API Renderer::Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList)
{	

}
void FIRECUBE_API Renderer::AddTechnique(const std::string &name, TechniquePtr technique)
{
    techniques[name] = technique;
}
TechniquePtr FIRECUBE_API Renderer::GetTechnique(const std::string &name)
{
    map<string, TechniquePtr>::iterator i = techniques.find(name);
    if (i != techniques.end())
        return i->second;
    return TechniquePtr();
}
void FIRECUBE_API Renderer::RemoveTechnique(const std::string &name)
{
    map<string, TechniquePtr>::iterator i = techniques.find(name);
    if (i != techniques.end())
        techniques.erase(i);
}
void FIRECUBE_API Renderer::UseCamera(CameraPtr camera)
{
	::camera = camera;
}
CameraPtr FIRECUBE_API Renderer::GetCamera()
{
	return camera;
}
unsigned int FIRECUBE_API Renderer::GetNumberOfTrianglesRendered()
{
	return numberOfTrianglesRendered;
}
void FIRECUBE_API Renderer::DisableVertexAttribute(int index)
{
	glDisableVertexAttribArray(index);
}