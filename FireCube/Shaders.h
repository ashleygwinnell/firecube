#ifndef SHADERS_H
#define SHADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
class ProgramResource;
class Program;
/* CPPDOC_BEGIN_EXCLUDE */
namespace Renderer
{
	void FIRECUBE_API UseProgram(Program program);
	void FIRECUBE_API SetGlobalProgram(Program program);
}
/* CPPDOC_END_EXCLUDE */

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
* A class Representing a single shader.
*/
class FIRECUBE_API Shader
{
	friend class ProgramResource;
	friend class ResourceManager<Shader,ShaderResource>;
public:
	Shader();
	Shader(boost::shared_ptr<ShaderResource> resource);
	/**
	* Loads a shader from a file. the shader type is determined by the extension of the file: .vert for a vertex shader, .frag for a fragment shader.
	* @param filename The file to load.
	*/
	bool Load(const std::string &filename);
	/**
	* Creates a shader from source.
	* @param shaderType The type of the shader.
	* @param source The source of the shader.
	*/
	bool Create(ShaderType type,const std::string &source);
	/**
	* Returns the resource id of the shader.
	*/
	unsigned int GetId() const;

	operator bool () const;
	bool operator== (const Shader &shader) const;
private:
	boost::shared_ptr<ShaderResource> resource;
};

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

	operator bool () const;
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
	* @return Returns true on success, false otherwise.
	*/
	bool LoadShader(const std::string &filename);

	/**
	* Generates a program from a given render state.
	* @param renderState The render state to use to generate the program.
	*/
	Program GenerateProgram(const RenderState &renderState);

	operator bool () const;
	bool operator== (const Technique &technique) const;

private:
	boost::shared_ptr<TechniqueResource> resource;
};

}
#endif