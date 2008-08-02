#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

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
		else if (ext=="pshader")
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
Program::Program() : id(0)
{

}
Program::~Program()
{
	glDeleteProgram(id);
}
void Program::Create()
{
	if (id!=0)
		glDeleteProgram(id);
	
	id=glCreateProgram();
	variables.clear();
}
void Program::Create(Shader shader1,Shader shader2)
{
	Create();
	Attach(shader1);
	Attach(shader2);
	Link();
}
void Program::Attach(Shader shader)
{
	glAttachShader(id,shader->id);
}
void Program::Link()
{
	glLinkProgram(id);	
}
void Program::Use(bool useShaders)
{
	if (useShaders==false)
		glUseProgram(0);
	else
		glUseProgram(id);
}
void Program::Uniform1f(const string &name,float value)
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
void Program::Uniform1i(const string &name,int value)
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
Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}
void Renderer::Clear()
{
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
			float ambient[]={j->material->ambient.x,j->material->ambient.y,j->material->ambient.z,1.0f};
			float diffuse[]={j->material->diffuse.x,j->material->diffuse.y,j->material->diffuse.z,1.0f};
			float specular[]={j->material->specular.x,j->material->specular.y,j->material->specular.z,1.0f};			
			glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
			glMaterialf(GL_FRONT,GL_SHININESS,j->material->shininess);
			if (j->material->tex)
			{
				glEnable(GL_TEXTURE_2D);
				UseTexture(j->material->tex,0);
			}
			else
				glDisable(GL_TEXTURE_2D);

			j->indexBuffer.RenderIndexStream(TRIANGLES,j->face.size()*3);
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
	Buffer vb,uvb;
	vb.Create();
	uvb.Create();
	int numQuads=0;
	vec2 *vBuffer=new vec2[str.size()*4];
	vec2 *uvBuffer=new vec2[str.size()*4];
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_TEXTURE_2D);
	UseTexture(font->page->tex,0);	
	vec2 curPos=pos;
	FT_Long useKerning = FT_HAS_KERNING( font->face ); 
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

			FT_UInt glyphIndex = FT_Get_Char_Index( font->face, c ); 
			/* retrieve kerning distance and move pen position */  
			if ( useKerning && previous && glyphIndex ) 
			{ 
				FT_Vector delta; 
				FT_Get_Kerning( font->face, previous, glyphIndex, FT_KERNING_DEFAULT, &delta ); 
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
	vb.RenderStream(QUADS,numQuads*4);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);	
	delete [] vBuffer;
	delete [] uvBuffer;
}