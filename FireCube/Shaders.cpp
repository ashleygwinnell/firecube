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
#include "Font.h"
#include "ShaderGenerator.h"
#include "Application.h"
using namespace FireCube;

ShaderResource::ShaderResource() : id(0)
{	
}
ShaderResource::~ShaderResource()
{
	glDeleteShader(id);
}
Shader::Shader()
{

}
Shader::Shader(boost::shared_ptr<ShaderResource> resource)
{
	this->resource=resource;
}
bool Shader::Load(const string &filename)
{
	resource=boost::shared_ptr<ShaderResource>(new ShaderResource);
	GLenum shaderType;
	if (resource->id!=0)
		glDeleteShader(resource->id);

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

	resource->id=glCreateShader(shaderType);
	ifstream f(filename.c_str(),ios::in|ios::binary|ios::ate);
	if (!f.is_open())
	{		
		return false;
	}	
	unsigned int l=(unsigned int)f.tellg();
	char *buffer=new char[l+1];
	f.seekg(0,ios_base::beg);
	f.read(buffer,l);
	buffer[l]=0;
	glShaderSource(resource->id,1,(const char**)&buffer,NULL);		
	glCompileShader(resource->id);
	delete [] buffer;

	return true;
}
Shader::operator bool () const
{
	return resource;
}
bool Shader::operator== (const Shader &shader) const
{
	return shader.resource==resource;
}
bool Shader::Create(ShaderType type,const string &source)
{
	resource=boost::shared_ptr<ShaderResource>(new ShaderResource);
	GLenum glShaderType;
	if (resource->id!=0)
		glDeleteShader(resource->id);

	if (type==VERTEX_SHADER)
		glShaderType=GL_VERTEX_SHADER;
	else if (type==FRAGMENT_SHADER)
		glShaderType=GL_FRAGMENT_SHADER;
	else
		return false;

	const char *buffer=source.c_str();
	resource->id=glCreateShader(glShaderType);	
	glShaderSource(resource->id,1,(const char**)&buffer,NULL);		
	glCompileShader(resource->id);	

	return true;

}
unsigned int Shader::GetId() const
{
	return resource->id;
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
void Program::Create()
{
	resource=boost::shared_ptr<ProgramResource>(new ProgramResource);
	if (resource->id!=0)
		glDeleteProgram(resource->id);

	resource->id=glCreateProgram();
	resource->variables.clear();
	ostringstream ss;
	ss<< "Created program with id="<<resource->id<<endl;
	Logger::Write(ss.str());
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
	glAttachShader(resource->id,shader.GetId());
}
void Program::Link()
{
	resource->variables.clear();
	glLinkProgram(resource->id);	
}
void Program::SetUniform(const string &name,float value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)
		glUniform1f(location,value);
}
void Program::SetUniform(const string &name,int value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)
		glUniform1i(location,value);
}
void Program::SetUniform(const string &name,vec3 value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)
		glUniform3fv(location,1,&value.x);
}
void Program::SetUniform(const string &name,vec4 value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)
		glUniform4fv(location,1,&value.x);
}
void Program::SetUniform(const string &name,bool value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)	
		glUniform1i(location,value);
}
void Program::SetUniform(const string &name,const vector<bool> &value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	vector<int> tmp;
	for (vector<bool>::const_iterator i=value.begin();i!=value.end();i++)
		tmp.push_back(*i);
	if (location!=-1)	
		glUniform1iv(location,value.size(),&tmp[0]);
}
void Program::SetUniform(const string &name,const vector<int> &value)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetUniformLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)	
		glUniform1iv(location,value.size(),&value[0]);
}
string Program::GetInfoLog()
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	string ret;
	glGetProgramiv(resource->id, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(resource->id, infologLength, &charsWritten, infoLog);
		ret=infoLog;
		delete [] infoLog;
	}
	return ret;

}
bool Program::IsValid() const
{
	return resource->id!=0;
}
unsigned int Program::GetId() const
{
	return resource->id;
}
Program::operator bool () const
{
	return resource;
}
bool Program::operator== (const Program &program) const
{
	return program.resource==resource;
}