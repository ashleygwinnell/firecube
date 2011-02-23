#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"

#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "Renderer.h"
#include "RenderQueue.h"
#include "Application.h"
#include "privateFont.h"
#include "Light.h"
#include "Node.h"

using namespace FireCube;
Program textShader;
Buffer textVertexBuffer;
Buffer textUvBuffer;
TextureManager *currentTextureManager=NULL;
ShaderManager *currentShaderManager=NULL;
FontManager *currentFontManager=NULL;
Program globalProgram;
map<string, Technique> techniques;
RenderState::RenderState() : directionalLighting(false), diffuseTexture(false), fog(false),pointLighting(false),normalTexture(false)
{

}
void RenderState::FromMaterial(Material mat)
{
	if (mat.GetDiffuseTexture())
		this->diffuseTexture=true;
	if (mat.GetNormalTexture())
		this->normalTexture=true;
}
void RenderState::SetDirectionalLighting(bool enable)
{
	directionalLighting=enable;
}
void RenderState::SetPointLighting(bool enable)
{
	pointLighting=enable;
}

void RenderState::SetFog(bool enable)
{
	fog=enable;
}
unsigned int RenderState::ToInt() const
{
	unsigned int ret=0;
	if (directionalLighting)
		ret|=1;
	if (fog)
		ret|=2;
	if (diffuseTexture)
		ret|=4;
	if (pointLighting)
		ret|=8;
	if (normalTexture)
		ret|=16;
	return ret;
}

void Renderer::Clear(vec4 color,float depth)
{
	glClearColor(color.x,color.y,color.z,color.w);
	glClearDepth(depth);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::SetModelViewMatrix(mat4 &m)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m.m);
}
void Renderer::SetTextureMatrix(mat4 &m,int unit)
{
	glActiveTexture(GL_TEXTURE0+unit);
	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(m.m);
}
void Renderer::SetProjectionMatrix(mat4 &m)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m.m);
}
void FIRECUBE_API Renderer::Render(Geometry geometry)
{
	if (!geometry)
		return;

	vector<Surface>::iterator i=geometry.resource->surface.begin();
	if (geometry.resource->vertexBuffer)
		geometry.resource->vertexBuffer.SetVertexStream(3);
	if (geometry.resource->diffuseUVBuffer)
		geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
	else
		Renderer::DisableTexCoordStream(0);
	if (geometry.resource->normalBuffer)
		geometry.resource->normalBuffer.SetNormalStream();
	else
		Renderer::DisableNormalStream();
	for (;i!=geometry.resource->surface.end();i++)
	{
		Renderer::UseMaterial(i->material);
		if (i->indexBuffer)
			i->indexBuffer.SetIndexStream();
		Renderer::RenderIndexStream(TRIANGLES,i->face.size()*3);
	}

}
void Renderer::SaveModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
void Renderer::RestoreModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void Renderer::SaveProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
}
void Renderer::RestoreProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void Renderer::MultiplyModelViewMatrix(mat4 &mat)
{
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(mat.m);
}
void Renderer::UseTexture(Texture tex,unsigned int unit)
{
	if (!tex)
		return;
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D,tex.resource->id);
}
void Renderer::RenderText(Font font,vec3 pos,vec4 color,const string &str)
{	
	if (!font)
		return;	
	static vector<vec3> vBuffer;
	static vector<vec2> uvBuffer;
	vBuffer.resize(str.size()*4);
	uvBuffer.resize(str.size()*4);	
	if (textShader.IsValid())
	{
		UseProgram(textShader);	
		textShader.SetUniform("tex0",0);
		textShader.SetUniform("textColor",color);
	}	
	int numQuads=0;
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
	UseTexture(font.resource->page->tex,0);		
	vec3 curPos=pos;	
	FT_Long useKerning = FT_HAS_KERNING(font.resource->fontImpl->face);
	FT_UInt previous = 0; 
	for (string::const_iterator i=str.begin();i!=str.end();i++)
	{
		char c=*i;
		if (c==32)
		{
			curPos.x+=font.resource->glyph[c].advance;
			continue;
		}
		if (font.resource->glyph[c].size!=vec2(0,0))
		{

			FT_UInt glyphIndex = FT_Get_Char_Index( font.resource->fontImpl->face, c ); 
			/* retrieve kerning distance and move pen position */  
			if ( useKerning && previous && glyphIndex ) 
			{ 
				FT_Vector delta; 
				FT_Get_Kerning( font.resource->fontImpl->face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta ); 
				curPos.x += delta.x >> 6; 
			} 
			vBuffer[numQuads*4+0]=vec3(font.resource->glyph[c].bitmapOffset.x+curPos.x,font.resource->glyph[c].bitmapOffset.y+curPos.y,curPos.z);
			vBuffer[numQuads*4+1]=vec3(font.resource->glyph[c].bitmapOffset.x+curPos.x,font.resource->glyph[c].bitmapOffset.y+curPos.y+font.resource->glyph[c].size.y,curPos.z);
			vBuffer[numQuads*4+2]=vec3(font.resource->glyph[c].bitmapOffset.x+curPos.x+font.resource->glyph[c].size.x,font.resource->glyph[c].bitmapOffset.y+curPos.y+font.resource->glyph[c].size.y,curPos.z);
			vBuffer[numQuads*4+3]=vec3(font.resource->glyph[c].bitmapOffset.x+curPos.x+font.resource->glyph[c].size.x,font.resource->glyph[c].bitmapOffset.y+curPos.y,curPos.z);
			uvBuffer[numQuads*4+0]=vec2(font.resource->glyph[c].uv.x,font.resource->glyph[c].uv.y);
			uvBuffer[numQuads*4+1]=vec2(font.resource->glyph[c].uv.x,font.resource->glyph[c].uv.y+font.resource->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+2]=vec2(font.resource->glyph[c].uv.x+font.resource->glyph[c].size.x/512.0f,font.resource->glyph[c].uv.y+font.resource->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+3]=vec2(font.resource->glyph[c].uv.x+font.resource->glyph[c].size.x/512.0f,font.resource->glyph[c].uv.y);
			numQuads++;
			curPos.x+=font.resource->glyph[c].advance;
			previous=glyphIndex;
		}		
	}	
	textVertexBuffer.LoadData(&vBuffer[0],numQuads*4*sizeof(vec3),STREAM);
	textUvBuffer.LoadData(&uvBuffer[0],numQuads*4*sizeof(vec2),STREAM);
	textUvBuffer.SetTexCoordStream(0);
	textVertexBuffer.SetVertexStream(3);
	RenderStream(QUADS,numQuads*4);
	glDisable(GL_BLEND);	
	glEnable(GL_DEPTH_TEST);		
}
void Renderer::RenderIndexStream(RenderMode mode,unsigned int count)
{
	GLenum glmode;
	switch (mode)
	{
	case POINTS:
		glmode=GL_POINTS;
		break;
	case LINES:
		glmode=GL_LINES;
		break;
	case TRIANGLES:
		glmode=GL_TRIANGLES;
		break;
	case TRIANGLE_STRIP:
		glmode=GL_TRIANGLE_STRIP;
		break;
	case QUADS:
		glmode=GL_QUADS;
		break;
	case LINE_LOOP:
		glmode=GL_LINE_LOOP;
		break;
	case TRIANGLE_FAN:
		glmode=GL_TRIANGLE_FAN;
		break;
	}	
	glDrawElements(glmode,count,GL_UNSIGNED_INT,0);
}
void Renderer::RenderStream(RenderMode mode,unsigned int count)
{
	GLenum glmode;
	switch (mode)
	{
	case POINTS:
		glmode=GL_POINTS;
		break;
	case LINES:
		glmode=GL_LINES;
		break;
	case TRIANGLES:
		glmode=GL_TRIANGLES;
		break;
	case TRIANGLE_STRIP:
		glmode=GL_TRIANGLE_STRIP;
		break;
	case QUADS:
		glmode=GL_QUADS;
		break;
	case LINE_LOOP:
		glmode=GL_LINE_LOOP;
		break;
	case TRIANGLE_FAN:
		glmode=GL_TRIANGLE_FAN;
		break;
	}	
	glDrawArrays(glmode,0,count);
}
void Renderer::UseProgram(Program program)
{
	if ((globalProgram) && (globalProgram.IsValid()))
		return;
	if (program)
		glUseProgram(program.GetId());	
}
void Renderer::SetGlobalProgram(Program program)
{
	globalProgram=program;
	if (program)
		glUseProgram(program.GetId());
}
void Renderer::UseMaterial(Material material)
{
	if (!material)
		return;
	
	vec4 ambientColor=material.GetAmbientColor();
	vec4 diffuseColor=material.GetDiffuseColor();
	vec4 specularColor=material.GetSpecularColor();
	glMaterialfv(GL_FRONT,GL_AMBIENT,&ambientColor.x);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,&diffuseColor.x);
	glMaterialfv(GL_FRONT,GL_SPECULAR,&specularColor.x);
	glMaterialf(GL_FRONT,GL_SHININESS,material.GetShininess());
	if ((material.GetDiffuseTexture()) && (material.GetDiffuseTexture().IsValid()))
	{
		UseTexture(material.GetDiffuseTexture(),0);
	}
	if ((material.GetNormalTexture()) && (material.GetNormalTexture().IsValid()))
	{
		UseTexture(material.GetNormalTexture(),1);
	}
}
void Renderer::SetPerspectiveProjection(float fov,float zNear,float zFar)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	mat4 p;
	p.GeneratePerspective(fov,(float)viewport[2]/(float)viewport[3],zNear,zFar);
	SetProjectionMatrix(p);
}
void Renderer::SetOrthographicProjection()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	mat4 p;
	p.GenerateOrthographic(0,(float)viewport[2],(float)viewport[3],0,0,1);
	SetProjectionMatrix(p);
}
void InitializeRenderer()
{	
	Shader vShader;
	Shader fShader;
	
	textVertexBuffer.Create();
	textUvBuffer.Create();
	vShader.Create(VERTEX_SHADER,"void main() \
								  {	\
								  gl_TexCoord[0]=gl_MultiTexCoord0; \
								  gl_Position = ftransform(); \
								  } ");

	fShader.Create(FRAGMENT_SHADER,"uniform vec4 textColor; \
									uniform sampler2D tex0; \
									void main() \
									{ \
									float alpha=texture2D(tex0,gl_TexCoord[0].st).a; \
									gl_FragColor = vec4(textColor.r,textColor.g,textColor.b,textColor.a*alpha);  \
									} ");
	textShader.Create(vShader,fShader);

	Technique technique;
	technique.Create();
	if (!technique.LoadShader("default.vert") && !technique.LoadShader("Shaders/default.vert") && !technique.LoadShader("../Assets/Shaders/default.vert") && !technique.LoadShader("./Assets/Shaders/default.vert"))
		Logger::Write(Logger::LOG_WARNING, "Could not load default technique's vertex shader");
	else
	{
		if (!technique.LoadShader("default.frag") && !technique.LoadShader("Shaders/default.frag") && !technique.LoadShader("../Assets/Shaders/default.frag") && !technique.LoadShader("./Assets/Shaders/default.frag"))
			Logger::Write(Logger::LOG_WARNING, "Could not load default technique's fragment shader");
		else
			Renderer::AddTechnique("default",technique);
	}
}
void Renderer::SetTextureManager(TextureManager &textureManager)
{
	currentTextureManager=&textureManager;
}
TextureManager &Renderer::GetTextureManager()
{
	return *currentTextureManager;
}
void Renderer::SetShaderManager(ShaderManager &shaderManager)
{
	currentShaderManager=&shaderManager;
}
ShaderManager &Renderer::GetShaderManager()
{
	return *currentShaderManager;
}
void Renderer::SetFontManager(FontManager &fontManager)
{
	currentFontManager=&fontManager;
}
FontManager &Renderer::GetFontManager()
{
	return *currentFontManager;
}
void DestroyRenderer()
{
	textVertexBuffer.Destroy();
	textUvBuffer.Destroy();
	textShader=Program();
}
void Renderer::UseFrameBuffer(FrameBuffer frameBuffer)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer.resource->id);	
	glViewport(0,0,frameBuffer.resource->width,frameBuffer.resource->height);	
	bool found=false;
	for (int i=0;(i<MAX_TEXTURES) && (!found);i++)
		if ((frameBuffer.resource->texture[i]) && (frameBuffer.resource->texture[i].IsValid()))
			found=true;
	if (found)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_NONE);
}
void Renderer::RestoreFrameBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);	
}
void Renderer::SetViewport(int left,int right,int width,int height)
{
	glViewport(left,right,width,height);
}
mat4 Renderer::GetModelViewMatrix()
{
	mat4 ret;
	glGetFloatv(GL_MODELVIEW_MATRIX,ret.m);
	return ret;
}
mat4 Renderer::GetProjectionMatrix()
{
	mat4 ret;
	glGetFloatv(GL_PROJECTION_MATRIX,ret.m);
	return ret;

}
void Renderer::DisableTexCoordStream(unsigned int unit)
{
	glClientActiveTexture(GL_TEXTURE0+unit);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void Renderer::DisableNormalStream()
{
	glDisableClientState(GL_NORMAL_ARRAY);
}
void RenderNode(Node node,vector<pair<mat4,pair<Node,Geometry>>> &renderingQueue,vector<pair<mat4,Light>> &activeLights)
{
	if (node.GetParent())
		node.SetWorldTransformation(node.GetParent().GetWorldTransformation()*node.GetLocalTransformation());
	else
		node.SetWorldTransformation(node.GetLocalTransformation());
	for (vector<Geometry>::iterator i=node.GetGeometries().begin();i!=node.GetGeometries().end();i++)
	{		
		renderingQueue.push_back(make_pair(node.GetWorldTransformation(),make_pair(node,*i)));
	}
	for (vector<Light>::iterator i=node.GetLights().begin();i!=node.GetLights().end();i++)
	{		
		activeLights.push_back(make_pair(node.GetWorldTransformation(),*i));
	}
	for (vector<Node>::iterator i=node.GetChildren().begin();i!=node.GetChildren().end();i++)
		RenderNode(*i,renderingQueue,activeLights);
}
void FIRECUBE_API Renderer::Render(Node node)
{
	vector<pair<mat4,pair<Node,Geometry>>> renderingQueue;
	vector<pair<mat4,Light>> activeLights;	
	bool firstPass=true;
	//root->Render();
	RenderNode(node,renderingQueue,activeLights);
	vector<pair<mat4,Light>>::iterator i;
	vector<pair<mat4,pair<Node,Geometry>>>::iterator j;
	glEnable(GL_BLEND);

	for (i=activeLights.begin();i!=activeLights.end();i++)
	{				
		if (firstPass)
		{
			glDepthFunc(GL_LESS);
			glBlendFunc(GL_ONE,GL_ZERO);
			glDepthMask(true);
		}
		else
		{
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_ONE,GL_ONE);
			glDepthMask(false);
		}		
		for (j=renderingQueue.begin();j!=renderingQueue.end();j++)
		{
			Geometry geometry=j->second.second;
			Node node=j->second.first;
			mat4 transformation=j->first;				
			if (node.GetRenderParameters().lighting)
			{							
				Renderer::SaveModelViewMatrix();
				Renderer::MultiplyModelViewMatrix(transformation);
				//Renderer::Render(geometry);

				vector<Surface>::iterator k=geometry.resource->surface.begin();
				if (geometry.resource->vertexBuffer)
					geometry.resource->vertexBuffer.SetVertexStream(3);
				if (geometry.resource->diffuseUVBuffer)
					geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
				else
					Renderer::DisableTexCoordStream(0);
				if (geometry.resource->normalBuffer)
					geometry.resource->normalBuffer.SetNormalStream();
				else
					Renderer::DisableNormalStream();
				for (;k!=geometry.resource->surface.end();k++)
				{
					Program p;
					Renderer::UseMaterial(k->material);
					if (node.GetProgram() && node.GetProgram().IsValid())
						p=node.GetProgram();
					else
					{
						RenderState rs;
						rs.FromMaterial(k->material);
						if (node.GetRenderParameters().lighting)
						{
							if (i->second.GetType()==DIRECTIONAL)
								rs.SetDirectionalLighting(true);				
							else if (i->second.GetType()==POINT)
								rs.SetPointLighting(true);
						}

						rs.SetFog(node.GetRenderParameters().fog);
						Technique technique=node.GetTechnique();
						if (technique)
							p=technique.GenerateProgram(rs);
						if (!p)
							break;
						Renderer::UseProgram(p);
						if (node.GetRenderParameters().lighting)
						{					
							if (i->second.GetType()==DIRECTIONAL)
							{
								p.SetUniform("directionalLightDir",vec3(0,0,1).TransformNormal(i->first));
								p.SetUniform("lightAmbient",i->second.GetAmbientColor());
								p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
								p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
							}
							else if (i->second.GetType()==POINT)
							{
								p.SetUniform("lightPosition",vec3(0,0,0) * i->first);
								p.SetUniform("lightAmbient",i->second.GetAmbientColor());
								p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
								p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
							}
						}
						if (node.GetRenderParameters().fog)
						{
							if (firstPass)
								p.SetUniform("fogColor",node.GetRenderParameters().fogColor);
							else
								p.SetUniform("fogColor",vec4(0,0,0,0));
							p.SetUniform("fogDensity",node.GetRenderParameters().fogDensity);
						}	
					}
					Renderer::UseProgram(p);
					if (geometry.resource->tangentBuffer)
						p.SetAttribute("atrTangent",geometry.resource->tangentBuffer,3);
					if (geometry.resource->bitangentBuffer)
						p.SetAttribute("atrBitangent",geometry.resource->bitangentBuffer,3);
					if (k->material.GetDiffuseTexture())
						p.SetUniform("diffuseMap",0);
					if (k->material.GetNormalTexture())
						p.SetUniform("normalMap",1);
					if (k->indexBuffer)
						k->indexBuffer.SetIndexStream();
					Renderer::RenderIndexStream(TRIANGLES,k->face.size()*3);
					
				}
				Renderer::RestoreModelViewMatrix();
			}
		}
		firstPass=false;
	}
	glDepthMask(true);
	glDisable(GL_BLEND);
	for (j=renderingQueue.begin();j!=renderingQueue.end();j++)
	{
		Geometry geometry=j->second.second;
		Node node=j->second.first;
		mat4 transformation=j->first;								
		if (!node.GetRenderParameters().lighting)
		{			
			Renderer::SaveModelViewMatrix();
			Renderer::MultiplyModelViewMatrix(transformation);			
			//Renderer::Render(geometry);

			vector<Surface>::iterator k=geometry.resource->surface.begin();
			if (geometry.resource->vertexBuffer)
				geometry.resource->vertexBuffer.SetVertexStream(3);
			if (geometry.resource->diffuseUVBuffer)
				geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
			else
				Renderer::DisableTexCoordStream(0);
			if (geometry.resource->normalBuffer)
				geometry.resource->normalBuffer.SetNormalStream();
			else
				Renderer::DisableNormalStream();
			for (;k!=geometry.resource->surface.end();k++)
			{
				Program p;
				Renderer::UseMaterial(k->material);
				if (node.GetProgram() && node.GetProgram().IsValid())
					p=node.GetProgram();
				else
				{
					RenderState rs;
					rs.FromMaterial(k->material);					

					rs.SetFog(node.GetRenderParameters().fog);
					rs.SetDirectionalLighting(false);
					rs.SetPointLighting(false);
					Technique technique=node.GetTechnique();
					if (technique)
						p=technique.GenerateProgram(rs);	
					if (!p)
						break;
					Renderer::UseProgram(p);					
					if (node.GetRenderParameters().fog)
					{
						p.SetUniform("fogColor",node.GetRenderParameters().fogColor);
						p.SetUniform("fogDensity",node.GetRenderParameters().fogDensity);
					}	
				}
				Renderer::UseProgram(p);
				if (geometry.resource->tangentBuffer)
					p.SetAttribute("atrTangent",geometry.resource->tangentBuffer,3);
				if (geometry.resource->bitangentBuffer)
					p.SetAttribute("atrBitangent",geometry.resource->bitangentBuffer,3);
				if (k->material.GetDiffuseTexture())
					p.SetUniform("diffuseMap",0);
				if (k->indexBuffer)
					k->indexBuffer.SetIndexStream();				
				Renderer::RenderIndexStream(TRIANGLES,k->face.size()*3);
			}	
			Renderer::RestoreModelViewMatrix();
		}
	}
}
void FIRECUBE_API Renderer::Render(Node node, const string &techniqueName, ProgramUniformsList &programUniformsList)
{
	vector<pair<mat4,pair<Node,Geometry>>> renderingQueue;
	vector<pair<mat4,Light>> activeLights;	
	bool firstPass=true;
	//root->Render();
	RenderNode(node,renderingQueue,activeLights);
	vector<pair<mat4,Light>>::iterator i;
	vector<pair<mat4,pair<Node,Geometry>>>::iterator j;
	glEnable(GL_BLEND);

	for (i=activeLights.begin();i!=activeLights.end();i++)
	{				
		if (firstPass)
		{
			glDepthFunc(GL_LESS);
			glBlendFunc(GL_ONE,GL_ZERO);
			glDepthMask(true);
		}
		else
		{
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_ONE,GL_ONE);
			glDepthMask(false);
		}		
		for (j=renderingQueue.begin();j!=renderingQueue.end();j++)
		{
			Geometry geometry=j->second.second;
			Node node=j->second.first;
			mat4 transformation=j->first;				
			if (node.GetRenderParameters().lighting)
			{							
				Renderer::SaveModelViewMatrix();
				Renderer::MultiplyModelViewMatrix(transformation);
				//Renderer::Render(geometry);

				vector<Surface>::iterator k=geometry.resource->surface.begin();
				if (geometry.resource->vertexBuffer)
					geometry.resource->vertexBuffer.SetVertexStream(3);
				if (geometry.resource->diffuseUVBuffer)
					geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
				else
					Renderer::DisableTexCoordStream(0);
				if (geometry.resource->normalBuffer)
					geometry.resource->normalBuffer.SetNormalStream();
				else
					Renderer::DisableNormalStream();
				for (;k!=geometry.resource->surface.end();k++)
				{
					Program p;
					Renderer::UseMaterial(k->material);
					
					RenderState rs;
					rs.FromMaterial(k->material);
					if (node.GetRenderParameters().lighting)
					{
						if (i->second.GetType()==DIRECTIONAL)
							rs.SetDirectionalLighting(true);				
						else if (i->second.GetType()==POINT)
							rs.SetPointLighting(true);
					}

					rs.SetFog(node.GetRenderParameters().fog);
					Technique technique=GetTechnique(techniqueName);
					if (technique)
						p=technique.GenerateProgram(rs);
					if (!p)
						break;
					Renderer::UseProgram(p);

					programUniformsList.ApplyForProgram(p);
					if (node.GetRenderParameters().lighting)
					{					
						if (i->second.GetType()==DIRECTIONAL)
						{
							p.SetUniform("directionalLightDir",vec3(0,0,1).TransformNormal(i->first));
							p.SetUniform("lightAmbient",i->second.GetAmbientColor());
							p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
							p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
						}
						else if (i->second.GetType()==POINT)
						{
							p.SetUniform("lightPosition",vec3(0,0,0) * i->first);
							p.SetUniform("lightAmbient",i->second.GetAmbientColor());
							p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
							p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
						}
					}
					if (node.GetRenderParameters().fog)
					{
						if (firstPass)
							p.SetUniform("fogColor",node.GetRenderParameters().fogColor);
						else
							p.SetUniform("fogColor",vec4(0,0,0,0));
						p.SetUniform("fogDensity",node.GetRenderParameters().fogDensity);
					}	
										
					if (geometry.resource->tangentBuffer)
						p.SetAttribute("atrTangent",geometry.resource->tangentBuffer,3);
					if (geometry.resource->bitangentBuffer)
						p.SetAttribute("atrBitangent",geometry.resource->bitangentBuffer,3);
					if (k->material.GetDiffuseTexture())
						p.SetUniform("diffuseMap",0);
					if (k->material.GetNormalTexture())
						p.SetUniform("normalMap",1);
					if (k->indexBuffer)
						k->indexBuffer.SetIndexStream();
					Renderer::RenderIndexStream(TRIANGLES,k->face.size()*3);

				}
				Renderer::RestoreModelViewMatrix();
			}
		}
		firstPass=false;
	}
	glDepthMask(true);
	glDisable(GL_BLEND);
	for (j=renderingQueue.begin();j!=renderingQueue.end();j++)
	{
		Geometry geometry=j->second.second;
		Node node=j->second.first;
		mat4 transformation=j->first;								
		if (!node.GetRenderParameters().lighting)
		{			
			Renderer::SaveModelViewMatrix();
			Renderer::MultiplyModelViewMatrix(transformation);			
			//Renderer::Render(geometry);

			vector<Surface>::iterator k=geometry.resource->surface.begin();
			if (geometry.resource->vertexBuffer)
				geometry.resource->vertexBuffer.SetVertexStream(3);
			if (geometry.resource->diffuseUVBuffer)
				geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
			else
				Renderer::DisableTexCoordStream(0);
			if (geometry.resource->normalBuffer)
				geometry.resource->normalBuffer.SetNormalStream();
			else
				Renderer::DisableNormalStream();
			for (;k!=geometry.resource->surface.end();k++)
			{
				Program p;
				Renderer::UseMaterial(k->material);
				
				RenderState rs;
				rs.FromMaterial(k->material);					

				rs.SetFog(node.GetRenderParameters().fog);
				rs.SetDirectionalLighting(false);
				rs.SetPointLighting(false);
				Technique technique=GetTechnique(techniqueName);
				if (technique)
					p=technique.GenerateProgram(rs);	
				if (!p)
					break;
				Renderer::UseProgram(p);
				
				programUniformsList.ApplyForProgram(p);
				if (node.GetRenderParameters().fog)
				{
					p.SetUniform("fogColor",node.GetRenderParameters().fogColor);
					p.SetUniform("fogDensity",node.GetRenderParameters().fogDensity);
				}					
								
				if (geometry.resource->tangentBuffer)
					p.SetAttribute("atrTangent",geometry.resource->tangentBuffer,3);
				if (geometry.resource->bitangentBuffer)
					p.SetAttribute("atrBitangent",geometry.resource->bitangentBuffer,3);
				if (k->material.GetDiffuseTexture())
					p.SetUniform("diffuseMap",0);
				if (k->indexBuffer)
					k->indexBuffer.SetIndexStream();				
				Renderer::RenderIndexStream(TRIANGLES,k->face.size()*3);
			}	
			Renderer::RestoreModelViewMatrix();
		}
	}
}
void FIRECUBE_API Renderer::Render(RenderQueue &renderQueue)
{
	bool firstPass=true;
	vector<pair<mat4,Light>>::iterator i;
	vector<RenderJob>::iterator j;
	glEnable(GL_BLEND);

	for (i=renderQueue.activeLights.begin();i!=renderQueue.activeLights.end();i++)
	{				
		if (firstPass)
		{
			glDepthFunc(GL_LESS);
			glBlendFunc(GL_ONE,GL_ZERO);
			glDepthMask(true);
		}
		else
		{
			glDepthFunc(GL_LEQUAL);
			glBlendFunc(GL_ONE,GL_ONE);
			glDepthMask(false);
		}		
		for (j=renderQueue.renderJobs.begin();j!=renderQueue.renderJobs.end();j++)
		{
			if (j->renderParameters.lighting)
			{							
				Renderer::SaveModelViewMatrix();
				Renderer::MultiplyModelViewMatrix(j->transformation);
				//Renderer::Render(geometry);
				
				if (j->geometry.resource->vertexBuffer)
					j->geometry.resource->vertexBuffer.SetVertexStream(3);
				if (j->geometry.resource->diffuseUVBuffer)
					j->geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
				else
					Renderer::DisableTexCoordStream(0);
				if (j->geometry.resource->normalBuffer)
					j->geometry.resource->normalBuffer.SetNormalStream();
				else
					Renderer::DisableNormalStream();
				Program p;
				Renderer::UseMaterial(j->material);
				if (j->renderParameters.program && j->renderParameters.program.IsValid())
					p=j->renderParameters.program;
				else
				{
					RenderState rs;
					rs.FromMaterial(j->material);
					if (j->renderParameters.lighting)
					{
						if (i->second.GetType()==DIRECTIONAL)
							rs.SetDirectionalLighting(true);				
						else if (i->second.GetType()==POINT)
							rs.SetPointLighting(true);
					}

					rs.SetFog(j->renderParameters.fog);
					Technique technique=j->renderParameters.technique;
					if (technique)
						p=technique.GenerateProgram(rs);
					if (!p)
						break;
					Renderer::UseProgram(p);
					if (j->renderParameters.lighting)
					{					
						if (i->second.GetType()==DIRECTIONAL)
						{
							p.SetUniform("directionalLightDir",vec3(0,0,1).TransformNormal(i->first));
							p.SetUniform("lightAmbient",i->second.GetAmbientColor());
							p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
							p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
						}
						else if (i->second.GetType()==POINT)
						{
							p.SetUniform("lightPosition",vec3(0,0,0) * i->first);
							p.SetUniform("lightAmbient",i->second.GetAmbientColor());
							p.SetUniform("lightDiffuse",i->second.GetDiffuseColor());
							p.SetUniform("lightSpecular",i->second.GetSpecularColor());								
						}
					}
					if (j->renderParameters.fog)
					{
						if (firstPass)
							p.SetUniform("fogColor",j->renderParameters.fogColor);
						else
							p.SetUniform("fogColor",vec4(0,0,0,0));
						p.SetUniform("fogDensity",j->renderParameters.fogDensity);
					}						
					Renderer::UseProgram(p);
					if (j->geometry.resource->tangentBuffer)
						p.SetAttribute("atrTangent",j->geometry.resource->tangentBuffer,3);
					if (j->geometry.resource->bitangentBuffer)
						p.SetAttribute("atrBitangent",j->geometry.resource->bitangentBuffer,3);
					if (j->material.GetDiffuseTexture())
						p.SetUniform("diffuseMap",0);
					if (j->material.GetNormalTexture())
						p.SetUniform("normalMap",1);
					if (j->surface->indexBuffer)
						j->surface->indexBuffer.SetIndexStream();
					Renderer::RenderIndexStream(TRIANGLES,j->surface->face.size()*3);

				}
				Renderer::RestoreModelViewMatrix();
			}
		}
		firstPass=false;
	}
	glDepthMask(true);
	glDisable(GL_BLEND);
	for (j=renderQueue.renderJobs.begin();j!=renderQueue.renderJobs.end();j++)
	{
		if (!j->renderParameters.lighting)
		{			
			Renderer::SaveModelViewMatrix();
			Renderer::MultiplyModelViewMatrix(j->transformation);			
			//Renderer::Render(geometry);

			vector<Surface>::iterator k=j->geometry.resource->surface.begin();
			if (j->geometry.resource->vertexBuffer)
				j->geometry.resource->vertexBuffer.SetVertexStream(3);
			if (j->geometry.resource->diffuseUVBuffer)
				j->geometry.resource->diffuseUVBuffer.SetTexCoordStream(0);
			else
				Renderer::DisableTexCoordStream(0);
			if (j->geometry.resource->normalBuffer)
				j->geometry.resource->normalBuffer.SetNormalStream();
			else
				Renderer::DisableNormalStream();
			
			Program p;
			Renderer::UseMaterial(j->material);
			if (j->renderParameters.program && j->renderParameters.program.IsValid())
				p=j->renderParameters.program;
			else
			{
				RenderState rs;
				rs.FromMaterial(j->material);					
				rs.SetFog(j->renderParameters.fog);
				rs.SetDirectionalLighting(false);
				rs.SetPointLighting(false);
				Technique technique=j->renderParameters.technique;
				if (technique)
					p=technique.GenerateProgram(rs);	
				if (!p)
					break;
				Renderer::UseProgram(p);					
				if (j->renderParameters.fog)
				{
					p.SetUniform("fogColor",j->renderParameters.fogColor);
					p.SetUniform("fogDensity",j->renderParameters.fogDensity);
				}					
				Renderer::UseProgram(p);
				if (j->geometry.resource->tangentBuffer)
					p.SetAttribute("atrTangent",j->geometry.resource->tangentBuffer,3);
				if (j->geometry.resource->bitangentBuffer)
					p.SetAttribute("atrBitangent",j->geometry.resource->bitangentBuffer,3);
				if (j->material.GetDiffuseTexture())
					p.SetUniform("diffuseMap",0);
				if (j->surface->indexBuffer)
					j->surface->indexBuffer.SetIndexStream();				
				Renderer::RenderIndexStream(TRIANGLES,j->surface->face.size()*3);
			}	
			Renderer::RestoreModelViewMatrix();
		}
	}
}
void FIRECUBE_API Renderer::AddTechnique(const std::string &name, Technique technique)
{
	techniques[name]=technique;
}
Technique FIRECUBE_API Renderer::GetTechnique(const std::string &name)
{
	map<string, Technique>::iterator i=techniques.find(name);
	if (i!=techniques.end())
		return i->second;
	return Technique();
}
void FIRECUBE_API Renderer::RemoveTechnique(const std::string &name)
{
	map<string, Technique>::iterator i=techniques.find(name);
	if (i!=techniques.end())	
		techniques.erase(i);	
}