#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
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
		string ext=filename.substr(d+1);
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
		i->vertexBuffer.SetVertexStream();
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
				glBindTexture(GL_TEXTURE_2D,j->material->tex->id);
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