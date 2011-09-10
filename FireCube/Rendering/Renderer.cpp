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
#include "Dependencies/GLee.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/ResourceManager.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Texture.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shaders.h"
#include "Geometry/Geometry.h"
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
ProgramPtr textShader;
BufferPtr textVertexBuffer;
BufferPtr textUvBuffer;
TextureManager *currentTextureManager = NULL;
ShaderManager *currentShaderManager = NULL;
FontManager *currentFontManager = NULL;
ProgramPtr globalProgram;
map<string, TechniquePtr> techniques;
CameraPtr camera;
unsigned int numberOfTrianglesRendered = 0;
RenderState::RenderState() : directionalLighting(false), diffuseTexture(false), fog(false), pointLighting(false), normalTexture(false)
{

}
void RenderState::FromMaterial(MaterialPtr mat)
{
    if (mat->GetDiffuseTexture())
        this->diffuseTexture = true;
    if (mat->GetNormalTexture())
        this->normalTexture = true;
}
void RenderState::SetDirectionalLighting(bool enable)
{
    directionalLighting = enable;
}
void RenderState::SetPointLighting(bool enable)
{
    pointLighting = enable;
}

void RenderState::SetFog(bool enable)
{
    fog = enable;
}
unsigned int RenderState::ToInt() const
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
void Renderer::SetModelViewMatrix(const mat4 &m)
{    
    glLoadMatrixf(m.m);
}
void Renderer::SetTextureMatrix(const mat4 &m, int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(m.m);
	glMatrixMode(GL_MODELVIEW);
}
void Renderer::SetProjectionMatrix(const mat4 &m)
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m.m);
	glMatrixMode(GL_MODELVIEW);
}
void FIRECUBE_API Renderer::Render(GeometryPtr geometry)
{
    if (!geometry)
        return;
    
    if (geometry->vertexBuffer)
        geometry->vertexBuffer->SetVertexStream(3);
    if (geometry->diffuseUVBuffer)
        geometry->diffuseUVBuffer->SetTexCoordStream(0);
    else
        Renderer::DisableTexCoordStream(0);
    if (geometry->normalBuffer)
        geometry->normalBuffer->SetNormalStream();
    else
        Renderer::DisableNormalStream();
    
	Renderer::UseMaterial(geometry->material);
	if (geometry->indexBuffer)
		geometry->indexBuffer->SetIndexStream();
	Renderer::RenderIndexStream(TRIANGLES, geometry->face.size() * 3);
}
void Renderer::SaveModelViewMatrix()
{    
    glPushMatrix();
}
void Renderer::RestoreModelViewMatrix()
{    
    glPopMatrix();
}
void Renderer::SaveProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void Renderer::RestoreProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Renderer::MultiplyModelViewMatrix(const mat4 &mat)
{    
    glMultMatrixf(mat.m);
}
void Renderer::UseTexture(TexturePtr tex, unsigned int unit)
{
    if (!tex)
        return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);
}
void Renderer::RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const string &str)
{
    if (!font)
        return;
	if (str.empty())
		return;
    static vector<vec3> vBuffer;
    static vector<vec2> uvBuffer;
    vBuffer.resize(str.size() * 4);
    uvBuffer.resize(str.size() * 4);
    if (textShader->IsValid())
    {
        UseProgram(textShader);
        textShader->SetUniform("tex0", 0);
        textShader->SetUniform("textColor", color);
    }
    int numQuads = 0;

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
            vBuffer[numQuads * 4 + 0] = vec3(font->glyph[c].bitmapOffset.x + curPos.x, font->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
            vBuffer[numQuads * 4 + 1] = vec3(font->glyph[c].bitmapOffset.x + curPos.x, font->glyph[c].bitmapOffset.y + curPos.y + font->glyph[c].size.y, curPos.z);
            vBuffer[numQuads * 4 + 2] = vec3(font->glyph[c].bitmapOffset.x + curPos.x + font->glyph[c].size.x, font->glyph[c].bitmapOffset.y + curPos.y + font->glyph[c].size.y, curPos.z);
            vBuffer[numQuads * 4 + 3] = vec3(font->glyph[c].bitmapOffset.x + curPos.x + font->glyph[c].size.x, font->glyph[c].bitmapOffset.y + curPos.y, curPos.z);
            uvBuffer[numQuads * 4 + 0] = vec2(font->glyph[c].uv.x, font->glyph[c].uv.y);
            uvBuffer[numQuads * 4 + 1] = vec2(font->glyph[c].uv.x, font->glyph[c].uv.y + font->glyph[c].size.y / 512.0f);
            uvBuffer[numQuads * 4 + 2] = vec2(font->glyph[c].uv.x + font->glyph[c].size.x / 512.0f, font->glyph[c].uv.y + font->glyph[c].size.y / 512.0f);
            uvBuffer[numQuads * 4 + 3] = vec2(font->glyph[c].uv.x + font->glyph[c].size.x / 512.0f, font->glyph[c].uv.y);
            numQuads++;
            curPos.x += font->glyph[c].advance;
            previous = glyphIndex;
        }
    }
    textVertexBuffer->LoadData(&vBuffer[0], numQuads * 4 * sizeof(vec3), STREAM);
    textUvBuffer->LoadData(&uvBuffer[0], numQuads * 4 * sizeof(vec2), STREAM);
    textUvBuffer->SetTexCoordStream(0);
    textVertexBuffer->SetVertexStream(3);
    RenderStream(QUADS, numQuads * 4);
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
    case TRIANGLE_FAN:
        glmode = GL_TRIANGLE_FAN;
        break;
    }
    glDrawElements(glmode, count, GL_UNSIGNED_INT, 0);
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
    case TRIANGLE_FAN:
        glmode = GL_TRIANGLE_FAN;
        break;
    }
    glDrawArrays(glmode, 0, count);
}
void Renderer::UseProgram(ProgramPtr program)
{
    if ((globalProgram) && (globalProgram->IsValid()))
        return;
    if (program)
        glUseProgram(program->GetId());
}
void Renderer::UseMaterial(MaterialPtr material)
{
    if (!material)
        return;

    vec4 ambientColor = vec4(material->GetAmbientColor().x, material->GetAmbientColor().y, material->GetAmbientColor().z, 1.0f);
    vec4 diffuseColor = vec4(material->GetDiffuseColor().x, material->GetDiffuseColor().y, material->GetDiffuseColor().z, 1.0f);
    vec4 specularColor = vec4(material->GetSpecularColor().x, material->GetSpecularColor().y, material->GetSpecularColor().z, 1.0f);
    glMaterialfv(GL_FRONT, GL_AMBIENT, &ambientColor.x);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, &diffuseColor.x);
    glMaterialfv(GL_FRONT, GL_SPECULAR, &specularColor.x);
    glMaterialf(GL_FRONT, GL_SHININESS, material->GetShininess());
    if ((material->GetDiffuseTexture()) && (material->GetDiffuseTexture()->IsValid()))
    {
        UseTexture(material->GetDiffuseTexture(), 0);
    }
    if ((material->GetNormalTexture()) && (material->GetNormalTexture()->IsValid()))
    {
        UseTexture(material->GetNormalTexture(), 1);
    }
}
void Renderer::SetPerspectiveProjection(float fov, float zNear, float zFar)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    mat4 p;
    p.GeneratePerspective(fov, (float)viewport[2] / (float)viewport[3], zNear, zFar);
    SetProjectionMatrix(p);
}
void Renderer::SetOrthographicProjection()
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    mat4 p;
    p.GenerateOrthographic(0, (float)viewport[2], (float)viewport[3], 0, 0, 1);
    SetProjectionMatrix(p);
}
void ResetNumberOfTrianglesRendered()
{
	numberOfTrianglesRendered = 0;
}
void InitializeRenderer()
{
    ShaderPtr vShader(new Shader);
    ShaderPtr fShader(new Shader);

    textShader = ProgramPtr(new Program);
    textVertexBuffer = BufferPtr(new Buffer);
    textVertexBuffer->Create();
    textUvBuffer = BufferPtr(new Buffer);
    textUvBuffer->Create();
    vShader->Create(VERTEX_SHADER, "void main() \
								  {	\
								  gl_TexCoord[0]=gl_MultiTexCoord0; \
								  gl_Position = ftransform(); \
								  } ");

    fShader->Create(FRAGMENT_SHADER, "uniform vec4 textColor; \
									uniform sampler2D tex0; \
									void main() \
									{ \
									float alpha=texture2D(tex0,gl_TexCoord[0].st).a; \
									gl_FragColor = vec4(textColor.r,textColor.g,textColor.b,textColor.a*alpha);  \
									} ");
    textShader->Create(vShader, fShader);

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
	Renderer::SetCamera(CameraPtr(new Camera));
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
    textUvBuffer = BufferPtr();
    textShader = ProgramPtr();
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
mat4 Renderer::GetModelViewMatrix()
{
    mat4 ret;
    glGetFloatv(GL_MODELVIEW_MATRIX, ret.m);
    return ret;
}
mat4 Renderer::GetProjectionMatrix()
{
    mat4 ret;
    glGetFloatv(GL_PROJECTION_MATRIX, ret.m);
    return ret;

}
void Renderer::DisableTexCoordStream(unsigned int unit)
{
    glClientActiveTexture(GL_TEXTURE0 + unit);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void Renderer::DisableNormalStream()
{
    glDisableClientState(GL_NORMAL_ARRAY);
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
	Renderer::SetModelViewMatrix(camera->GetViewMatrix());
	Renderer::SetProjectionMatrix(camera->GetProjectionMatrix());
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
				RenderState rs;
				rs.FromMaterial(j->geometry->GetMaterial());
				if (i->second.GetType() == DIRECTIONAL)
					rs.SetDirectionalLighting(true);
				else if (i->second.GetType() == POINT)
					rs.SetPointLighting(true);					
				rs.SetFog(j->renderParameters.fog);
				TechniquePtr technique = j->renderParameters.technique;
				if (technique)
					j->program = technique->GenerateProgram(rs);					
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
			Renderer::SaveModelViewMatrix();
			Renderer::MultiplyModelViewMatrix(j->transformation);				

			ProgramPtr p;
			Renderer::UseMaterial(j->geometry->GetMaterial());
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
				if (j->geometry->GetMaterial()->GetDiffuseTexture())
					p->SetUniform("diffuseMap", 0);
				if (j->geometry->GetMaterial()->GetNormalTexture())
					p->SetUniform("normalMap", 1);
			}
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
				if (j->geometry->vertexBuffer)
					j->geometry->vertexBuffer->SetVertexStream(3);
				if (j->geometry->diffuseUVBuffer)
					j->geometry->diffuseUVBuffer->SetTexCoordStream(0);
				else
					Renderer::DisableTexCoordStream(0);
				if (j->geometry->normalBuffer)
					j->geometry->normalBuffer->SetNormalStream();
				else
					Renderer::DisableNormalStream();											
				if (j->geometry->tangentBuffer)
					p->SetAttribute("atrTangent", j->geometry->tangentBuffer, 3);
				if (j->geometry->bitangentBuffer)
					p->SetAttribute("atrBitangent", j->geometry->bitangentBuffer, 3);
				if (j->geometry->indexBuffer)					
					j->geometry->indexBuffer->SetIndexStream();

			}
			Renderer::RenderIndexStream(j->geometry->GetPrimitiveType(), j->geometry->GetIndexCount());
			numberOfTrianglesRendered += j->geometry->face.size();

			Renderer::RestoreModelViewMatrix();			
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
			RenderState rs;
			rs.FromMaterial(j->geometry->GetMaterial());
			rs.SetDirectionalLighting(false);
			rs.SetPointLighting(false);					
			rs.SetFog(j->renderParameters.fog);
			TechniquePtr technique = j->renderParameters.technique;
			if (technique)
				j->program = technique->GenerateProgram(rs);					
		}				
	}
	renderQueue.Sort(RenderQueue::NON_LIGHTED);
	for (j = renderQueue.renderJobs[RenderQueue::NON_LIGHTED].begin(); j != renderQueue.renderJobs[RenderQueue::NON_LIGHTED].end(); j++)
	{		
		Renderer::SaveModelViewMatrix();
		Renderer::MultiplyModelViewMatrix(j->transformation);


		ProgramPtr p;
		Renderer::UseMaterial(j->geometry->GetMaterial());
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
			if (j->geometry->GetMaterial()->GetDiffuseTexture())
				p->SetUniform("diffuseMap", 0);
			if (j->geometry->GetMaterial()->GetNormalTexture())
				p->SetUniform("normalMap", 1);
		}			
		if (lastGeometry != j->geometry)
		{
			lastGeometry = j->geometry;
			if (j->geometry->vertexBuffer)
				j->geometry->vertexBuffer->SetVertexStream(3);
			if (j->geometry->diffuseUVBuffer)
				j->geometry->diffuseUVBuffer->SetTexCoordStream(0);
			else
				Renderer::DisableTexCoordStream(0);
			if (j->geometry->normalBuffer)
				j->geometry->normalBuffer->SetNormalStream();
			else
				Renderer::DisableNormalStream();			

			if (j->geometry->tangentBuffer)
				p->SetAttribute("atrTangent", j->geometry->tangentBuffer, 3);
			if (j->geometry->bitangentBuffer)
				p->SetAttribute("atrBitangent", j->geometry->bitangentBuffer, 3);
			if (j->geometry->indexBuffer)
				j->geometry->indexBuffer->SetIndexStream();
		}			

		Renderer::RenderIndexStream(j->geometry->GetPrimitiveType(), j->geometry->GetIndexCount());
		numberOfTrianglesRendered += j->geometry->face.size();			
		Renderer::RestoreModelViewMatrix();		
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
void FIRECUBE_API Renderer::SetCamera(CameraPtr camera)
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