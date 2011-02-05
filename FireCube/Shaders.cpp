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
#include "Font.h"
#include "RenderQueue.h"
#include "Renderer.h"
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
		if (ext=="vert")
			shaderType=GL_VERTEX_SHADER;
		else if (ext=="frag")
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
	ss<< "Destroyed program with id="<<id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteProgram(id);
	id=0;
}
void Program::Create()
{
	resource=boost::shared_ptr<ProgramResource>(new ProgramResource);
	
	resource->id=glCreateProgram();
	resource->variables.clear();
	ostringstream ss;
	ss<< "Created program with id="<<resource->id;
	Logger::Write(Logger::LOG_INFO, ss.str());
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
void Program::SetAttribute(const std::string &name,Buffer buffer,int size)
{
	GLint location=-1;
	map<string,GLint>::iterator i=resource->variables.find(name);
	if (i!=resource->variables.end())
		location=i->second;
	else
	{
		location=glGetAttribLocation(resource->id,name.c_str());
		if (location!=-1)
			resource->variables[name]=location;
	}
	if (location!=-1)
	{		
		buffer.Bind();
		glVertexAttribPointer(location,size,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(location);
	}
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

void Technique::Create()
{
	resource=boost::shared_ptr<TechniqueResource>(new TechniqueResource);
}
Technique::operator bool () const
{
	return resource;
}
bool Technique::operator== (const Technique &technique) const
{
	return technique.resource==resource;
}
bool Technique::LoadShader(const string &filename)
{
	string name=Application::SearchForFileName(filename);
	if (name.empty())
		return false;
	string *source;
	string::size_type d;
	d=filename.find_last_of(".");
	if (d!=string::npos)
	{
		string ext=ToLower(filename.substr(d+1));
		if (ext=="vert")
			source=&resource->vertexShaderCode;
		else if (ext=="frag")
			source=&resource->fragmentShaderCode;
		else
			return false;
	}
	ifstream f(name.c_str());
	if (!f.is_open())	
		return false;
		
	*source=string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());	
	return true;
}
Program Technique::GenerateProgram(const RenderState &renderState)
{
	unsigned int key=renderState.ToInt();
	map<unsigned int,Program>::iterator i=resource->programs.find(key);
	if (i!=resource->programs.end())
		return i->second;
	
	ostringstream defines;
	if (renderState.diffuseTexture)
		defines << "#define DIFFUSE_MAPPING" << endl;
	if (renderState.directionalLighting)
		defines << "#define DIRECTIONAL_LIGHTING" << endl;
	if (renderState.fog)
		defines << "#define FOG" << endl;
	if (renderState.normalTexture)
		defines << "#define NORMAL_MAPPING" << endl;
	if (renderState.pointLighting)
		defines << "#define POINT_LIGHTING" << endl;
	ostringstream vertexShaderSource;
	ostringstream fragmentShaderSource;
	Shader vertexShader,fragmentShader;
	vertexShaderSource << defines.str() << resource->vertexShaderCode;
	fragmentShaderSource << defines.str() << resource->fragmentShaderCode;
	Program p;
	vertexShader.Create(VERTEX_SHADER,vertexShaderSource.str());
	fragmentShader.Create(FRAGMENT_SHADER,fragmentShaderSource.str());
	p.Create(vertexShader,fragmentShader);
	resource->programs[key]=p;
	return p;
}