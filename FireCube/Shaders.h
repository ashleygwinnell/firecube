#ifndef SHADERS_H
#define SHADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
class ProgramResource;
class Program;

namespace Renderer
{
	void FIRECUBE_API UseProgram(Program program);
	void FIRECUBE_API SetGlobalProgram(Program program);
}

/** 
* Specifies the type of a shader.
*/
enum ShaderType
{
	VERTEX_SHADER,FRAGMENT_SHADER
};

class FIRECUBE_API ShaderResource
{
	friend class ProgramResource;
public:
	ShaderResource();
	~ShaderResource();	
	GLuint id;
};

/**
* A class representing a single shader.
*/
class FIRECUBE_API Shader
{
	friend class ProgramResource;
	friend class ResourceManager<Shader,ShaderResource>;
public:
	Shader();

	/**
	* Constructs a shader from a shader resource.
	* @param resource The resource to assign to this shader.
	*/
	Shader(boost::shared_ptr<ShaderResource> resource);
	/**
	* Loads a shader from a file. The shader type is determined by the extension of the file: .vert for a vertex shader, .frag for a fragment shader.
	* @param filename The file to load.
	*/
	bool Load(const std::string &filename);
	/**
	* Creates a shader from source.
	* @param type The type of the shader.
	* @param source The source of the shader.
	*/
	bool Create(ShaderType type,const std::string &source);
	/**
	* Returns the resource id of the shader.
	*/
	unsigned int GetId() const;

	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Shader &shader) const;
private:
	boost::shared_ptr<ShaderResource> resource;
};

/**
* A shader resource manager.
*/
typedef ResourceManager<Shader,ShaderResource> ShaderManager;

class FIRECUBE_API ProgramResource
{
	friend void Renderer::UseProgram(Program program);
	friend void Renderer::SetGlobalProgram(Program program);
public:
	ProgramResource();
	~ProgramResource();

	GLuint id;
	std::map<std::string,GLint> variables;
};

/**
* A class representing a gpu program.
*/
class FIRECUBE_API Program
{
	friend void Renderer::UseProgram(Program program);
	friend void Renderer::SetGlobalProgram(Program program);
public:
	
	/**
	* Creates the program.
	*/
	void Create();
	
	/**
	* Creates the program and attaches two shaders to it.
	*/
	void Create(Shader shader1,Shader shader2);
	
	/**
	* Attaches a shader to the program.
	* @param shader The shader to attach.
	*/
	void Attach(Shader shader);
	
	/**
	* Links the program.
	*/
	void Link();	
	
	/**
	* Sets a 1d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,float value);
	
	/**
	* Sets a 1d uniform integer.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,int value);
	
	/**
	* Sets a 2d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,vec2 value);

	/**
	* Sets a 3d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,vec3 value);
	
	/**
	* Sets a 4d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,vec4 value);

	/**
	* Sets a 3x3 uniform matrix.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,mat3 value);

	/**
	* Sets a 4x4 uniform matrix.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,mat4 value);
	
	/**
	* Sets a 1d uniform boolean.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,bool value);
	
	/**
	* Sets an array of booleans.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,const std::vector<bool> &value);
	
	/**
	* Sets an array of integers.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const std::string &name,const std::vector<int> &value);
	
	/**
	* Sets vertex shader attribute.
	* @param name The name of the variable.
	* @param buffer The buffer from which data will be read.
	* @param size The number of elements in each attribue.
	*/
	void SetAttribute(const std::string &name,Buffer buffer,int size);
	
	/**	
	* @return Returns the compile log for this program.
	*/
	std::string GetInfoLog();
	
	/**
	* Returns whether the program is valid.
	*/	
	bool IsValid() const;
	
	/**
	* Returns the resource id of the program.
	*/
	unsigned int GetId() const;
	
	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Program &program) const;

private:
	boost::shared_ptr<ProgramResource> resource;
};

class FIRECUBE_API TechniqueResource
{
public:
	std::map<unsigned int,Program> programs;
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
};
class RenderState;
/**
* A class representing a technique.
*/
class FIRECUBE_API Technique
{	
public:
	/**
	* Creates the technique.
	*/
	void Create();
	
	/**
	* Loads the specified shader.
	* @param filename The file name of the shader.
	* @return True on success, false otherwise.
	*/
	bool LoadShader(const std::string &filename);

	/**
	* Loads the specified shader.
	* @param type The type of the shader.
	* @param source The source of the shader.
	* @return True on success, false otherwise.
	*/
	bool LoadShader(ShaderType type,const std::string &source);

	/**
	* Generates a program from a given render state.
	* @param renderState The render state to use to generate the program.
	*/
	Program GenerateProgram(const RenderState &renderState);

	/**
	* @return True if points to an initialized resource, false otherwise.
	*/
	operator bool () const;

	/**
	* @return True of both objects points to the same resource, false otherwise.
	*/
	bool operator== (const Technique &technique) const;

private:
	boost::shared_ptr<TechniqueResource> resource;
};

/**
* A class storing a list of uniforms to be fed into a gpu program.
*/
class FIRECUBE_API ProgramUniformsList
{	
public:
	
	/**
	* Assigns an integer value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetIntValue(const std::string &name, int value);

	/**
	* Gets the value of an integer uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	int GetIntValue(const std::string &name);

	/**
	* Assigns a float value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetFloatValue(const std::string &name, float value);

	/**
	* Gets the value of a float uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	float GetFloatValue(const std::string &name);

	/**
	* Assigns a 2d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec2Value(const std::string &name, vec2 value);

	/**
	* Gets the value of a 2d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec2 GetVec2Value(const std::string &name);

	/**
	* Assigns a 3d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec3Value(const std::string &name, vec3 value);

	/**
	* Gets the value of a 3d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec3 GetVec3Value(const std::string &name);

	/**
	* Assigns a 4d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec4Value(const std::string &name, vec4 value);

	/**
	* Gets the value of a 4d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec4 GetVec4Value(const std::string &name);

	/**
	* Assigns a 3x3 matrix value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetMat3Value(const std::string &name, mat3 value);

	/**
	* Gets the value of a 3x3 matrix uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	mat3 GetMat3Value(const std::string &name);

	/**
	* Assigns a 4x4 matrix value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetMat4Value(const std::string &name, mat4 value);

	/**
	* Gets the value of a 4x4 matrix uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	mat4 GetMat4Value(const std::string &name);

	/**
	* Removes a variable from the list.
	* @param name The name of the variable to remove.
	*/
	void RemoveValue(const std::string &name);

	/**
	* Applies the list to a gpu program.
	* @param program The program to apply the list to.
	*/
	void ApplyForProgram(Program program);

private:
	std::map<std::string, int> intMap;
	std::map<std::string, float> floatMap;
	std::map<std::string, vec2> vec2Map;
	std::map<std::string, vec3> vec3Map;
	std::map<std::string, vec4> vec4Map;
	std::map<std::string, mat3> mat3Map;
	std::map<std::string, mat4> mat4Map;
};

}
#endif