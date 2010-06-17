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
#include "Mesh.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "Renderer.h"				
#include "Application.h"
#include "privateFont.h"
using namespace FireCube;
Program textShader;
Buffer textVertexBuffer;
Buffer textUvBuffer;
TextureManager *currentTextureManager=NULL;
ShaderManager *currentShaderManager=NULL;
FontManager *currentFontManager=NULL;
Program globalProgram;
ShaderResource::ShaderResource() : id(0)
{	
}
ShaderResource::~ShaderResource()
{
	glDeleteShader(id);
}
bool ShaderResource::Load(const string &filename)
{
	GLenum shaderType;
	if (id!=0)
		glDeleteShader(id);
	
	string::size_type d;
	d=filename.find_last_of(".");
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="vshader")
			shaderType=GL_VERTEX_SHADER;
		else if (ext=="fshader")
			shaderType=GL_FRAGMENT_SHADER;
		else
			return false;
	}

	id=glCreateShader(shaderType);
	ifstream f(filename.c_str(),ios::in|ios::binary|ios::ate);
	if (!f.is_open())
	{		
		return false;
	}	
	DWORD l=f.tellg();
	char *buffer=new char[l+1];
	f.seekg(0,ios_base::beg);
	f.read(buffer,l);
	buffer[l]=0;
	glShaderSource(id,1,(const char**)&buffer,NULL);		
	glCompileShader(id);
	delete [] buffer;

	return true;
}
bool ShaderResource::Create(ShaderType type,const string &source)
{
	GLenum glShaderType;
	if (id!=0)
		glDeleteShader(id);
	
	if (type==VERTEX_SHADER)
		glShaderType=GL_VERTEX_SHADER;
	else if (type==FRAGMENT_SHADER)
		glShaderType=GL_FRAGMENT_SHADER;
	else
			return false;
	
	const char *buffer=source.c_str();
	id=glCreateShader(glShaderType);	
	glShaderSource(id,1,(const char**)&buffer,NULL);		
	glCompileShader(id);	

	return true;

}
ProgramResource::ProgramResource() : id(0)
{

}
ProgramResource::~ProgramResource()
{
	ostringstream ss;
	ss<< "Destroyed program with id="<<id<<endl;
	Logger::Write(ss.str());
	glDeleteProgram(id);
	id=0;
}
void ProgramResource::Create()
{
	if (id!=0)
		glDeleteProgram(id);
	
	id=glCreateProgram();
	variables.clear();
	ostringstream ss;
	ss<< "Created program with id="<<id<<endl;
	Logger::Write(ss.str());
}
void ProgramResource::Create(Shader shader1,Shader shader2)
{
	Create();
	Attach(shader1);
	Attach(shader2);
	Link();
}
void ProgramResource::Attach(Shader shader)
{
	glAttachShader(id,shader->id);
}
void ProgramResource::Link()
{
	variables.clear();
	glLinkProgram(id);	
}
void ProgramResource::SetUniform(const string &name,float value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)
		glUniform1f(location,value);
}
void ProgramResource::SetUniform(const string &name,int value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)
		glUniform1i(location,value);
}
void ProgramResource::SetUniform(const string &name,vec3 value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)
		glUniform3fv(location,1,&value.x);
}
void ProgramResource::SetUniform(const string &name,vec4 value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)
		glUniform4fv(location,1,&value.x);
}
void ProgramResource::SetUniform(const string &name,bool value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)	
		glUniform1i(location,value);
}
void ProgramResource::SetUniform(const string &name,const vector<bool> &value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	vector<int> tmp;
	for (vector<bool>::const_iterator i=value.begin();i!=value.end();i++)
		tmp.push_back(*i);
	if (location!=-1)	
		glUniform1iv(location,value.size(),&tmp[0]);
}
void ProgramResource::SetUniform(const string &name,const vector<int> &value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=-1)
			variables[name]=location;
	}
	if (location!=-1)	
		glUniform1iv(location,value.size(),&value[0]);
}
string ProgramResource::GetInfoLog()
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	string ret;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(id, infologLength, &charsWritten, infoLog);
		ret=infoLog;
		delete [] infoLog;
	}
	return ret;

}
bool ProgramResource::IsValid() const
{
	return id!=0;
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
void Renderer::Render(Model model)
{
	if (!model)
		return;

	vector<Object>::iterator i=model->object.begin();	
	
	for (;i!=model->object.end();i++)
	{	
		vector<Mesh>::iterator j=i->mesh.begin();
		if (i->vertexBuffer)
			i->vertexBuffer->SetVertexStream(3);
		for (DWORD t=0;t<MAX_TEXTURES;t++)
			if ((i->uvBuffer[t]) && (i->uvBuffer[t]->IsValid()))
				i->uvBuffer[t]->SetTexCoordStream(t);
			else
				Renderer::DisableTexCoordStream(t);
		if (i->normalBuffer)
			i->normalBuffer->SetNormalStream();
		else
			Renderer::DisableNormalStream();
		for (;j!=i->mesh.end();j++)
		{
			UseMaterial(j->material);
			if (j->indexBuffer)
				j->indexBuffer->SetIndexStream();
			RenderIndexStream(TRIANGLES,j->face.size()*3);			
		}
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
	glBindTexture(GL_TEXTURE_2D,tex->id);
}
void Renderer::RenderText(Font font,vec3 pos,vec4 color,const string &str)
{	
	if (!font)
		return;	
	static vector<vec3> vBuffer;
	static vector<vec2> uvBuffer;
	vBuffer.resize(str.size()*4);
	uvBuffer.resize(str.size()*4);	
	if (textShader->IsValid())
	{
		UseProgram(textShader);	
		textShader->SetUniform("tex0",0);
		textShader->SetUniform("textColor",color);
	}	
	int numQuads=0;
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);	
	UseTexture(font->page->tex,0);		
	vec3 curPos=pos;	
	FT_Long useKerning = FT_HAS_KERNING(font->fontImpl->face);
	FT_UInt previous = 0; 
	for (string::const_iterator i=str.begin();i!=str.end();i++)
	{
		char c=*i;
		if (c==32)
		{
			curPos.x+=font->glyph[c].advance;
			continue;
		}
		if (font->glyph[c].size!=vec2(0,0))
		{

			FT_UInt glyphIndex = FT_Get_Char_Index( font->fontImpl->face, c ); 
			/* retrieve kerning distance and move pen position */  
			if ( useKerning && previous && glyphIndex ) 
			{ 
				FT_Vector delta; 
				FT_Get_Kerning( font->fontImpl->face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta ); 
				curPos.x += delta.x >> 6; 
			} 
			vBuffer[numQuads*4+0]=vec3(font->glyph[c].bitmapOffset.x+curPos.x,font->glyph[c].bitmapOffset.y+curPos.y,curPos.z);
			vBuffer[numQuads*4+1]=vec3(font->glyph[c].bitmapOffset.x+curPos.x,font->glyph[c].bitmapOffset.y+curPos.y+font->glyph[c].size.y,curPos.z);
			vBuffer[numQuads*4+2]=vec3(font->glyph[c].bitmapOffset.x+curPos.x+font->glyph[c].size.x,font->glyph[c].bitmapOffset.y+curPos.y+font->glyph[c].size.y,curPos.z);
			vBuffer[numQuads*4+3]=vec3(font->glyph[c].bitmapOffset.x+curPos.x+font->glyph[c].size.x,font->glyph[c].bitmapOffset.y+curPos.y,curPos.z);
			uvBuffer[numQuads*4+0]=vec2(font->glyph[c].uv.x,font->glyph[c].uv.y);
			uvBuffer[numQuads*4+1]=vec2(font->glyph[c].uv.x,font->glyph[c].uv.y+font->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+2]=vec2(font->glyph[c].uv.x+font->glyph[c].size.x/512.0f,font->glyph[c].uv.y+font->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+3]=vec2(font->glyph[c].uv.x+font->glyph[c].size.x/512.0f,font->glyph[c].uv.y);
			numQuads++;
			curPos.x+=font->glyph[c].advance;
			previous=glyphIndex;
		}		
	}	
	textVertexBuffer->LoadData(&vBuffer[0],numQuads*4*sizeof(vec3),STREAM);
	textUvBuffer->LoadData(&uvBuffer[0],numQuads*4*sizeof(vec2),STREAM);
	textUvBuffer->SetTexCoordStream(0);
	textVertexBuffer->SetVertexStream(3);
	RenderStream(QUADS,numQuads*4);
	glDisable(GL_BLEND);	
	glEnable(GL_DEPTH_TEST);		
}
void Renderer::RenderIndexStream(RenderMode mode,DWORD count)
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
void Renderer::RenderStream(RenderMode mode,DWORD count)
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
	if ((globalProgram) && (globalProgram->IsValid()))
		return;
	if (program)
		glUseProgram(program->id);	
}
void Renderer::SetGlobalProgram(Program program)
{
	globalProgram=program;
	if (program)
		glUseProgram(program->id);
}
void Renderer::UseMaterial(Material material)
{
	if (!material)
		return;
	
	glMaterialfv(GL_FRONT,GL_AMBIENT,&material->ambient.x);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,&material->diffuse.x);
	glMaterialfv(GL_FRONT,GL_SPECULAR,&material->specular.x);
	glMaterialf(GL_FRONT,GL_SHININESS,material->shininess);
	if ((material->diffuseTexture) && (material->diffuseTexture->IsValid()))
	{
		UseTexture(material->diffuseTexture,0);
	}
	if ((material->program) && (material->program->IsValid()))
	{
		UseProgram(material->program);
		material->program->SetUniform("diffuseMap",0);
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
	textShader=Program(new ProgramResource);
	Shader vShader=Shader(new ShaderResource);
	Shader fShader=Shader(new ShaderResource);				
	textVertexBuffer=Buffer(new BufferResource);
	textUvBuffer=Buffer(new BufferResource);

	textVertexBuffer->Create();
	textUvBuffer->Create();
	vShader->Create(VERTEX_SHADER,"void main() \
								  {	\
								  gl_TexCoord[0]=gl_MultiTexCoord0; \
								  gl_Position = ftransform(); \
								  } ");

	fShader->Create(FRAGMENT_SHADER,"uniform vec4 textColor; \
									uniform sampler2D tex0; \
									void main() \
									{ \
									float alpha=texture2D(tex0,gl_TexCoord[0].st).a; \
									gl_FragColor = vec4(textColor.r,textColor.g,textColor.b,textColor.a*alpha);  \
									} ");
	textShader->Create(vShader,fShader);		
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
	textVertexBuffer.reset();
	textUvBuffer.reset();
	textShader.reset();
}
void Renderer::UseFrameBuffer(FrameBuffer frameBuffer)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer->id);	
	glViewport(0,0,frameBuffer->width,frameBuffer->height);	
	bool found=false;
	for (int i=0;(i<MAX_TEXTURES) && (!found);i++)
		if ((frameBuffer->texture[i]) && (frameBuffer->texture[i]->IsValid()))
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
void Renderer::DisableTexCoordStream(DWORD unit)
{
	glClientActiveTexture(GL_TEXTURE0+unit);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
void Renderer::DisableNormalStream()
{
	glDisableClientState(GL_NORMAL_ARRAY);
}