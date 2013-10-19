#include <fstream>
#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

Shader::Shader(Renderer *renderer) : GraphicsResource(renderer)
{

}

Shader::~Shader()
{
	glDeleteShader(objectId);
}

bool Shader::Load(const std::string &filename)
{
	GLenum shaderType;
	if (objectId != 0)
		glDeleteShader(objectId);

	// Determine shader type from file extension
	std::string::size_type d;
	d = filename.find_last_of(".");
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		if (ext == "vert")
			shaderType = GL_VERTEX_SHADER;
		else if (ext == "frag")
			shaderType = GL_FRAGMENT_SHADER;
		else
			return false;
	}
	else
		return false;
	
	objectId = glCreateShader(shaderType);
	std::ifstream f(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!f.is_open())
	{
		return false;
	}
	unsigned int l = (unsigned int) f.tellg();
	char *buffer = new char[l + 1];
	f.seekg(0, std::ios_base::beg);
	f.read(buffer, l);
	buffer[l] = 0;
	glShaderSource(objectId, 1, (const char**)&buffer, nullptr);
	glCompileShader(objectId);
	delete [] buffer;

	GLint status;
	glGetShaderiv(objectId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(objectId, infoLogLength, nullptr, strInfoLog);

		LOGERROR(strInfoLog);
		delete[] strInfoLog;
	}

	return true;
}

bool Shader::Create(ShaderType type, const std::string &source)
{
	GLenum glShaderType;
	if (objectId != 0)
		glDeleteShader(objectId);

	if (type == VERTEX_SHADER)
		glShaderType = GL_VERTEX_SHADER;
	else if (type == FRAGMENT_SHADER)
		glShaderType = GL_FRAGMENT_SHADER;
	else
		return false;

	const char *buffer = source.c_str();
	objectId = glCreateShader(glShaderType);
	glShaderSource(objectId, 1, (const char**)&buffer, nullptr);
	glCompileShader(objectId);

	GLint status;
	glGetShaderiv(objectId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(objectId, infoLogLength, nullptr, strInfoLog);

		LOGERROR(strInfoLog);
		delete[] strInfoLog;
	}

	return true;

}

Program::Program(Renderer *renderer) : GraphicsResource(renderer)
{

}

Program::~Program()
{
	std::ostringstream ss;
	ss << "Destroyed program with id=" << objectId;
	LOGINFO(ss.str());
	glDeleteProgram(objectId);
	objectId = 0;
}

void Program::Create()
{
	objectId = glCreateProgram();
	variables.clear();
	std::ostringstream ss;
	ss << "Created program with id=" << objectId;
	LOGINFO(ss.str());
}

void Program::Create(Shader *shader1, Shader *shader2)
{
	Create();
	Attach(shader1);
	Attach(shader2);
	Link();
}

void Program::Attach(Shader *shader)
{
	glAttachShader(objectId, shader->GetObjectId());
}

void Program::Link()
{
	variables.clear();
	glLinkProgram(objectId);

	GLint status;
	glGetProgramiv(objectId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(objectId, infoLogLength, nullptr, strInfoLog);
		LOGERROR(strInfoLog);
		delete[] strInfoLog;
		return;
	}
	int uniformsCount;
	glGetProgramInterfaceiv(objectId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniformsCount);	
	std::vector<GLchar> nameData(256);
	std::vector<GLenum> properties;
	properties.push_back(GL_NAME_LENGTH);
	properties.push_back(GL_LOCATION);	
	std::vector<GLint> values(properties.size());
	for(int i = 0; i < uniformsCount; ++i)
	{
		glGetProgramResourceiv(objectId, GL_UNIFORM, i, properties.size(), &properties[0], values.size(), NULL, &values[0]);

		nameData.resize(values[0] + 1); //The length of the name.
		glGetProgramResourceName(objectId, GL_UNIFORM, i, nameData.size(), NULL, &nameData[0]);
		std::string name(nameData.begin(), nameData.end() - 1);		
		variables[StringHash(name)] = values[1];
	}
	glUseProgram(objectId);
	SetUniform(StringHash("diffuseMap"), 0);		
	SetUniform(StringHash("normalMap"), 1);

}

void Program::SetUniform(const StringHash &nameHash, float value)
{	
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform1f(i->second, value);			
}

void Program::SetUniform(const StringHash &nameHash, int value)
{	
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())		
		glUniform1i(i->second, value);	
}

void Program::SetUniform(const StringHash &nameHash, const vec2 &value)
{	
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform2fv(i->second, 1, &value.x);
}

void Program::SetUniform(const StringHash &nameHash, const vec3 &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform3fv(i->second, 1, &value.x);
}

void Program::SetUniform(const StringHash &nameHash, const vec4 &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform4fv(i->second, 1, &value.x);
}

void Program::SetUniform(const StringHash &nameHash, const mat3 &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniformMatrix3fv(i->second, 1, false, value.m);
}

void Program::SetUniform(const StringHash &nameHash, const mat4 &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniformMatrix4fv(i->second, 1, false, value.m);
}

void Program::SetUniform(const StringHash &nameHash, bool value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform1i(i->second, value);
}

void Program::SetUniform(const StringHash &nameHash, const std::vector<bool> &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
	{
		std::vector<int> tmp(value.size());
		for (unsigned int v = 0; v < value.size(); ++v)
			tmp[v] = value[v];
		
		glUniform1iv(i->second, value.size(), &tmp[0]);
	}
}

void Program::SetUniform(const StringHash &nameHash, const std::vector<int> &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i != variables.end())
		glUniform1iv(i->second, value.size(), &value[0]);
}

void Program::SetUniform(const StringHash &nameHash, const Variant &value)
{
	std::map<StringHash, GLint>::iterator i = variables.find(nameHash);
	if (i == variables.end())
		return;
	switch (value.GetType())
	{
	case VariantType::INT:
		glUniform1i(i->second, value.GetInt());
		break;
	case VariantType::FLOAT:
		glUniform1f(i->second, value.GetFloat());
		break;		
	case VariantType::BOOL:
		glUniform1i(i->second, value.GetBool());
		break;
	case VariantType::VEC2:
		glUniform2fv(i->second, 1, &value.GetVec2().x);
		break;
	case VariantType::VEC3:
		glUniform3fv(i->second, 1, &value.GetVec3().x);
		break;
	case VariantType::VEC4:
		glUniform4fv(i->second, 1, &value.GetVec4().x);
		break;
	default:
		break;
	}
}

void Program::SetAttribute(const std::string &name, VertexBufferPtr buffer, int size)
{
	GLint location = -1;
	std::map<StringHash, GLint>::iterator i = variables.find(StringHash(name));
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetAttribLocation(objectId, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)    
		buffer->SetVertexAttribute(location, size, 0, 0);        
	
}

std::string Program::GetInfoLog() const
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	std::string ret;
	glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(objectId, infologLength, &charsWritten, infoLog);
		ret = infoLog;
		delete [] infoLog;
	}
	return ret;

}
