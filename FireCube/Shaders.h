#ifndef SHADERS_H
#define SHADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

class ProgramResource;
typedef boost::shared_ptr<ProgramResource> Program;

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

/**
* A class Representing a single shader.
*/
class FIRECUBE_API ShaderResource
{
	friend class ProgramResource;
public:
	ShaderResource();
	~ShaderResource();
	/**
	* Loads a shader from a file. the shader type is determined by the extension of the file: .vshader for a vertex shader, .fshader for a fragment shader.
	* @param filename The file to load.
	*/
	bool Load(const string &filename);
	/**
	* Creates a shader from source.
	* @param shaderType The type of the shader.
	* @param source The source of the shader.
	*/
	bool Create(ShaderType type,const string &source);
private:
	GLuint id;
};

typedef boost::shared_ptr<ShaderResource> Shader;
typedef ResourceManager<ShaderResource> ShaderManager;

/**
* A class representing a gpu program.
*/
class FIRECUBE_API ProgramResource
{
	friend void Renderer::UseProgram(Program program);
	friend void Renderer::SetGlobalProgram(Program program);
public:
	ProgramResource();
	~ProgramResource();
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
	void SetUniform(const string &name,float value);
	/**
	* Sets a 1d uniform integer.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,int value);
	/**
	* Sets a 3d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,vec3 value);
	/**
	* Sets a 4d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,vec4 value);
	/**
	* Sets a 1d uniform boolean.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,bool value);
	/**
	* Sets an array of booleans.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,const vector<bool> &value);
	/**
	* Sets an array of integers.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const string &name,const vector<int> &value);
	/**	
	* @return Returns the compile log for this program.
	*/
	string GetInfoLog();
	/**
	* Returns whether the program is valid.
	*/	
	bool IsValid() const;	

private:
	GLuint id;
	map<string,GLint> variables;
};
#endif