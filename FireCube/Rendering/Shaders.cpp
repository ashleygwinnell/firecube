#include <fstream>
#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shaders.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

Shader::Shader() : id(0)
{

}

Shader::~Shader()
{
	glDeleteShader(id);
}

bool Shader::Load(const std::string &filename)
{
	GLenum shaderType;
	if (id != 0)
		glDeleteShader(id);

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
	
	id = glCreateShader(shaderType);
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
	glShaderSource(id, 1, (const char**)&buffer, nullptr);
	glCompileShader(id);
	delete [] buffer;

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(id, infoLogLength, nullptr, strInfoLog);

		Logger::Write(Logger::LOG_ERROR, strInfoLog);
		delete[] strInfoLog;
	}

	return true;
}

bool Shader::Create(ShaderType type, const std::string &source)
{
	GLenum glShaderType;
	if (id != 0)
		glDeleteShader(id);

	if (type == VERTEX_SHADER)
		glShaderType = GL_VERTEX_SHADER;
	else if (type == FRAGMENT_SHADER)
		glShaderType = GL_FRAGMENT_SHADER;
	else
		return false;

	const char *buffer = source.c_str();
	id = glCreateShader(glShaderType);
	glShaderSource(id, 1, (const char**)&buffer, nullptr);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(id, infoLogLength, nullptr, strInfoLog);

		Logger::Write(Logger::LOG_ERROR, strInfoLog);
		delete[] strInfoLog;
	}

	return true;

}

unsigned int Shader::GetId() const
{
	return id;
}

Program::Program() : id(0)
{

}

Program::~Program()
{
	std::ostringstream ss;
	ss << "Destroyed program with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteProgram(id);
	id = 0;
}

void Program::Create()
{
	id = glCreateProgram();
	variables.clear();
	std::ostringstream ss;
	ss << "Created program with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
}

void Program::Create(ShaderPtr shader1, ShaderPtr shader2)
{
	Create();
	Attach(shader1);
	Attach(shader2);
	Link();
}

void Program::Attach(ShaderPtr shader)
{
	glAttachShader(id, shader->GetId());
}

void Program::Link()
{
	variables.clear();
	glLinkProgram(id);

	GLint status;
	glGetProgramiv (id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(id, infoLogLength, nullptr, strInfoLog);
		Logger::Write(Logger::LOG_ERROR, strInfoLog);
		delete[] strInfoLog;
	}
}

void Program::SetUniform(const std::string &name, float value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform1f(location, value);
}

void Program::SetUniform(const std::string &name, int value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform1i(location, value);
}

void Program::SetUniform(const std::string &name, const vec2 &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform2fv(location, 1, &value.x);
}

void Program::SetUniform(const std::string &name, const vec3 &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform3fv(location, 1, &value.x);
}

void Program::SetUniform(const std::string &name, const vec4 &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform4fv(location, 1, &value.x);
}

void Program::SetUniform(const std::string &name, const mat3 &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniformMatrix3fv(location, 1, false, value.m);
}

void Program::SetUniform(const std::string &name, const mat4 &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniformMatrix4fv(location, 1, false, value.m);
}

void Program::SetUniform(const std::string &name, bool value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform1i(location, value);
}

void Program::SetUniform(const std::string &name, const std::vector<bool> &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	std::vector<int> tmp;
	for (std::vector<bool>::const_iterator i = value.begin(); i != value.end(); i++)
		tmp.push_back(*i);
	if (location != -1)
		glUniform1iv(location, value.size(), &tmp[0]);
}

void Program::SetUniform(const std::string &name, const std::vector<int> &value)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetUniformLocation(id, name.c_str());
		if (location != -1)
			variables[name] = location;
	}
	if (location != -1)
		glUniform1iv(location, value.size(), &value[0]);
}

void Program::SetAttribute(const std::string &name, BufferPtr buffer, int size)
{
	GLint location = -1;
	std::map<std::string, GLint>::iterator i = variables.find(name);
	if (i != variables.end())
		location = i->second;
	else
	{
		location = glGetAttribLocation(id, name.c_str());
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
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog(id, infologLength, &charsWritten, infoLog);
		ret = infoLog;
		delete [] infoLog;
	}
	return ret;

}

bool Program::IsValid() const
{
	return id != 0;
}

unsigned int Program::GetId() const
{
	return id;
}

bool Technique::LoadShader(const std::string &filename)
{
	std::string name = Filesystem::SearchForFileName(filename);
	if (name.empty())
		return false;
	std::string *source;
	std::string::size_type d;
	d = filename.find_last_of(".");
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		if (ext == "vert")
			source = &vertexShaderCode;
		else if (ext == "frag")
			source = &fragmentShaderCode;
		else
			return false;
	}
	else
		return false;

	std::ifstream f(name.c_str());
	if (!f.is_open())
		return false;

	*source = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return true;
}

bool Technique::LoadShader(ShaderType type, const std::string &source)
{
	if (type == VERTEX_SHADER)
		vertexShaderCode = source;
	else if (type == FRAGMENT_SHADER)
		fragmentShaderCode = source;
	else
		return false;

	return true;
}

ProgramPtr Technique::GenerateProgram(const ShaderProperties &shaderProperties)
{
	// Hash the shader properties and check if already generated
	unsigned int key = shaderProperties.ToInt();
	std::map<unsigned int, ProgramPtr>::iterator i = programs.find(key);
	if (i != programs.end())
		return i->second;

	// Add defines to the shader source according to the shader properties
	std::ostringstream defines;
	if (shaderProperties.diffuseTexture)
		defines << "#define DIFFUSE_MAPPING" << std::endl;
	if (shaderProperties.directionalLighting)
		defines << "#define DIRECTIONAL_LIGHTING" << std::endl;
	if (shaderProperties.fog)
		defines << "#define FOG" << std::endl;
	if (shaderProperties.normalTexture)
		defines << "#define NORMAL_MAPPING" << std::endl;
	if (shaderProperties.pointLighting)
		defines << "#define POINT_LIGHTING" << std::endl;
	std::ostringstream vertexShaderSource;
	std::ostringstream fragmentShaderSource;
	ShaderPtr vertexShader(new Shader), fragmentShader(new Shader);
	vertexShaderSource << defines.str() << vertexShaderCode;
	fragmentShaderSource << defines.str() << fragmentShaderCode;
	ProgramPtr p(new Program);
	vertexShader->Create(VERTEX_SHADER, vertexShaderSource.str());
	fragmentShader->Create(FRAGMENT_SHADER, fragmentShaderSource.str());
	p->Create(vertexShader, fragmentShader);
	programs[key] = p;
	return p;
}

void ProgramUniformsList::SetIntValue(const std::string &name, int value)
{
	intMap[name] = value;
}

int ProgramUniformsList::GetIntValue(const std::string &name) const
{
	std::map<std::string, int>::const_iterator i = intMap.find(name);
	if (i == intMap.end())
		return 0;
	return i->second;
}

void ProgramUniformsList::SetFloatValue(const std::string &name, float value)
{
	floatMap[name] = value;
}

float ProgramUniformsList::GetFloatValue(const std::string &name) const
{
	std::map<std::string, float>::const_iterator i = floatMap.find(name);
	if (i == floatMap.end())
		return 0.0f;
	return i->second;
}

void ProgramUniformsList::SetVec2Value(const std::string &name, const vec2 &value)
{
	vec2Map[name] = value;
}

vec2 ProgramUniformsList::GetVec2Value(const std::string &name) const
{
	std::map<std::string, vec2>::const_iterator i = vec2Map.find(name);
	if (i == vec2Map.end())
		return vec2();
	return i->second;
}

void ProgramUniformsList::SetVec3Value(const std::string &name, const vec3 &value)
{
	vec3Map[name] = value;
}

vec3 ProgramUniformsList::GetVec3Value(const std::string &name) const
{
	std::map<std::string, vec3>::const_iterator i = vec3Map.find(name);
	if (i == vec3Map.end())
		return vec3();
	return i->second;
}

void ProgramUniformsList::SetVec4Value(const std::string &name, const vec4 &value)
{
	vec4Map[name] = value;
}

vec4 ProgramUniformsList::GetVec4Value(const std::string &name) const
{
	std::map<std::string, vec4>::const_iterator i = vec4Map.find(name);
	if (i == vec4Map.end())
		return vec4();
	return i->second;
}

void ProgramUniformsList::SetMat3Value(const std::string &name, const mat3 &value)
{
	mat3Map[name] = value;
}

mat3 ProgramUniformsList::GetMat3Value(const std::string &name) const
{
	std::map<std::string, mat3>::const_iterator i = mat3Map.find(name);
	if (i == mat3Map.end())
		return mat3();
	return i->second;
}

void ProgramUniformsList::SetMat4Value(const std::string &name, const mat4 &value)
{
	mat4Map[name] = value;
}

mat4 ProgramUniformsList::GetMat4Value(const std::string &name) const
{
	std::map<std::string, mat4>::const_iterator i = mat4Map.find(name);
	if (i == mat4Map.end())
		return mat4();
	return i->second;
}

void ProgramUniformsList::RemoveValue(const std::string &name)
{
	std::map<std::string, int>::iterator i0 = intMap.find(name);
	if (i0 != intMap.end())
	{
		intMap.erase(i0);
		return;
	}
	std::map<std::string, float>::iterator i1 = floatMap.find(name);
	if (i1 != floatMap.end())
	{
		floatMap.erase(i1);
		return;
	}
	std::map<std::string, vec2>::iterator i2 = vec2Map.find(name);
	if (i2 != vec2Map.end())
	{
		vec2Map.erase(i2);
		return;
	}
	std::map<std::string, vec3>::iterator i3 = vec3Map.find(name);
	if (i3 != vec3Map.end())
	{
		vec3Map.erase(i3);
		return;
	}
	std::map<std::string, vec4>::iterator i4 = vec4Map.find(name);
	if (i4 != vec4Map.end())
	{
		vec4Map.erase(i4);
		return;
	}
	std::map<std::string, mat3>::iterator i5 = mat3Map.find(name);
	if (i5 != mat3Map.end())
	{
		mat3Map.erase(i5);
		return;
	}
	std::map<std::string, mat4>::iterator i6 = mat4Map.find(name);
	if (i6 != mat4Map.end())
	{
		mat4Map.erase(i6);
		return;
	}
}

void ProgramUniformsList::ApplyForProgram(ProgramPtr program) const
{
	Renderer::UseProgram(program);
	for (std::map<std::string, int>::const_iterator i = intMap.begin(); i != intMap.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, float>::const_iterator i = floatMap.begin(); i != floatMap.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, vec2>::const_iterator i = vec2Map.begin(); i != vec2Map.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, vec3>::const_iterator i = vec3Map.begin(); i != vec3Map.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, vec4>::const_iterator i = vec4Map.begin(); i != vec4Map.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, mat3>::const_iterator i = mat3Map.begin(); i != mat3Map.end(); i++)
		program->SetUniform(i->first, i->second);
	for (std::map<std::string, mat4>::const_iterator i = mat4Map.begin(); i != mat4Map.end(); i++)
		program->SetUniform(i->first, i->second);
}