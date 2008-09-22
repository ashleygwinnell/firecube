#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"
#include "FireCube.h"
#include "privateFont.h"
using namespace FireCube;
Program textShader;
TextureManager *currentTextureManager=NULL;
ShaderManager *currentShaderManager=NULL;
FontManager *currentFontManager=NULL;

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
	glLinkProgram(id);	
}
void ProgramResource::Uniform1f(const string &name,float value)
{
	GLint location=0;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=0)
			variables[name]=location;
	}
	if (location!=0)
		glUniform1f(location,value);
}
void ProgramResource::Uniform1i(const string &name,int value)
{
	GLint location=0;
	map<string,GLint>::iterator i=variables.find(name);
	if (i!=variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(id,name.c_str());
		if (location!=0)
			variables[name]=location;
	}
	if (location!=0)
		glUniform1i(location,value);
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
		i->vertexBuffer.SetVertexStream(3);
		i->uvBuffer.SetTexCoordStream(0);
		i->normalBuffer.SetNormalStream();
		for (;j!=i->mesh.end();j++)
		{
			UseMaterial(*j->material);
			j->indexBuffer.SetIndexStream();
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
	glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D,tex->id);
}
void Renderer::RenderText(Font font,vec2 pos,const string &str)
{	
	if (!font)
		return;	
	if (textShader->IsValid())
	{
		UseProgram(textShader);	
		textShader->Uniform1i("tex0",0);
	}
	Buffer vb,uvb;
	vb.Create();
	uvb.Create();
	int numQuads=0;
	vec2 *vBuffer=new vec2[str.size()*4];
	vec2 *uvBuffer=new vec2[str.size()*4];
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);	
	UseTexture(font->page->tex,0);	
	glEnable(GL_TEXTURE_2D);
	vec2 curPos=pos;	
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
			vBuffer[numQuads*4+0]=vec2(font->glyph[c].bitmapOffset.x+curPos.x,font->glyph[c].bitmapOffset.y+curPos.y);
			vBuffer[numQuads*4+1]=vec2(font->glyph[c].bitmapOffset.x+curPos.x,font->glyph[c].bitmapOffset.y+curPos.y+font->glyph[c].size.y);
			vBuffer[numQuads*4+2]=vec2(font->glyph[c].bitmapOffset.x+curPos.x+font->glyph[c].size.x,font->glyph[c].bitmapOffset.y+curPos.y+font->glyph[c].size.y);
			vBuffer[numQuads*4+3]=vec2(font->glyph[c].bitmapOffset.x+curPos.x+font->glyph[c].size.x,font->glyph[c].bitmapOffset.y+curPos.y);
			uvBuffer[numQuads*4+0]=vec2(font->glyph[c].uv.x,font->glyph[c].uv.y);
			uvBuffer[numQuads*4+1]=vec2(font->glyph[c].uv.x,font->glyph[c].uv.y+font->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+2]=vec2(font->glyph[c].uv.x+font->glyph[c].size.x/512.0f,font->glyph[c].uv.y+font->glyph[c].size.y/512.0f);
			uvBuffer[numQuads*4+3]=vec2(font->glyph[c].uv.x+font->glyph[c].size.x/512.0f,font->glyph[c].uv.y);
			numQuads++;
			curPos.x+=font->glyph[c].advance;
			previous=glyphIndex;
		}		
	}	
	vb.LoadData(vBuffer,numQuads*4*2*sizeof(float),STREAM);
	uvb.LoadData(uvBuffer,numQuads*4*2*sizeof(float),STREAM);
	uvb.SetTexCoordStream(0);
	vb.SetVertexStream(2);
	RenderStream(QUADS,numQuads*4);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);	
	delete [] vBuffer;
	delete [] uvBuffer;
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
	if (program)
		glUseProgram(program->id);	
}
void Renderer::UseMaterial(const Material &material)
{
	float ambient[]={material.ambient.x,material.ambient.y,material.ambient.z,1.0f};
	float diffuse[]={material.diffuse.x,material.diffuse.y,material.diffuse.z,1.0f};
	float specular[]={material.specular.x,material.specular.y,material.specular.z,1.0f};			
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,material.shininess);
	for (int i=0;i<MAX_TEXTURES;i++)
	{	
		if ((material.texture[i]) && (material.texture[i]->IsValid()))
		{
			UseTexture(material.texture[i],i);
			glEnable(GL_TEXTURE_2D);			
		}
		else
		{
			glActiveTexture(GL_TEXTURE0+i);
			glDisable(GL_TEXTURE_2D);
		}
	}

	if ((material.program) && (material.program->IsValid()))
		UseProgram(material.program);
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
	Shader vshader=Shader(new ShaderResource);
	Shader pshader=Shader(new ShaderResource);				

	vshader->Create(VERTEX_SHADER,"void main() \
								  {	\
								  gl_TexCoord[0]=gl_MultiTexCoord0; \
								  gl_Position = ftransform(); \
								  } ");

	pshader->Create(FRAGMENT_SHADER,"uniform sampler2D tex0; \
									void main() \
									{ \
									gl_FragColor = texture2D(tex0,gl_TexCoord[0].st);  \
									} ");
	textShader->Create(vshader,pshader);	
	
}
void Renderer::SetTextureManager(TextureManager *textureManager)
{
	currentTextureManager=textureManager;
}
TextureManager *Renderer::GetTextureManager()
{
	return currentTextureManager;
}
void Renderer::SetShaderManager(ShaderManager *shaderManager)
{
	currentShaderManager=shaderManager;
}
ShaderManager *Renderer::GetShaderManager()
{
	return currentShaderManager;
}
void Renderer::SetFontManager(FontManager *fontManager)
{
	currentFontManager=fontManager;
}
FontManager *Renderer::GetFontManager()
{
	return currentFontManager;
}
void DestroyRenderer()
{
	textShader.reset();
}