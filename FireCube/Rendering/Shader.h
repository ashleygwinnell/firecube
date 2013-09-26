#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "Rendering/RenderingTypes.h"
#include "Rendering/GraphicsResource.h"
#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Utils/StringHash.h"

namespace FireCube
{

// Forward declarations.
class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
class Program;
class Renderer;

/**
* A shared pointer to a Program.
*/
typedef std::shared_ptr<Program> ProgramPtr;

/**
* A class representing a single shader.
*/
class FIRECUBE_API Shader : public GraphicsResource
{
	friend class Program;
public:
	Shader(Renderer *renderer);
	~Shader();

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
	bool Create(ShaderType type, const std::string &source);

private:
	
};

/**
* A shared pointer to a Shader.
*/
typedef std::shared_ptr<Shader> ShaderPtr;

/**
* A class representing a gpu program.
*/
class FIRECUBE_API Program : public GraphicsResource
{
	friend class Renderer;   
public:
	Program(Renderer *renderer);
	~Program();

	/**
	* Creates the program.
	*/
	void Create();

	/**
	* Creates the program and attaches two shaders to it.
	*/
	void Create(Shader *shader1, Shader *shader2);

	/**
	* Attaches a shader to the program.
	* @param shader The shader to attach.
	*/
	void Attach(Shader *shader);

	/**
	* Links the program.
	*/
	void Link();

	/**
	* Sets a 1d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, float value);

	/**
	* Sets a 1d uniform integer.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, int value);

	/**
	* Sets a 2d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const vec2 &value);

	/**
	* Sets a 3d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const vec3 &value);

	/**
	* Sets a 4d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const vec4 &value);

	/**
	* Sets a 3x3 uniform matrix.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const mat3 &value);

	/**
	* Sets a 4x4 uniform matrix.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const mat4 &value);

	/**
	* Sets a 1d uniform boolean.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, bool value);

	/**
	* Sets an array of booleans.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const std::vector<bool> &value);

	/**
	* Sets an array of integers.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const std::vector<int> &value);

	/**
	* Sets vertex shader attribute.
	* @param name The name of the variable.
	* @param buffer The buffer from which data will be read.
	* @param size The number of elements in each attribute.
	*/
	void SetAttribute(const std::string &name, VertexBufferPtr buffer, int size);

	/**
	* @return Returns the compile log for this program.
	*/
	std::string GetInfoLog() const;

private:    
	std::map<StringHash, GLint> variables;
};

class ShaderProperties;

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
	int GetIntValue(const std::string &name) const;

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
	float GetFloatValue(const std::string &name) const;

	/**
	* Assigns a 2d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec2Value(const std::string &name, const vec2 &value);

	/**
	* Gets the value of a 2d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec2 GetVec2Value(const std::string &name) const;

	/**
	* Assigns a 3d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec3Value(const std::string &name, const vec3 &value);

	/**
	* Gets the value of a 3d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec3 GetVec3Value(const std::string &name) const;

	/**
	* Assigns a 4d vector value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetVec4Value(const std::string &name, const vec4 &value);

	/**
	* Gets the value of a 4d vector uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	vec4 GetVec4Value(const std::string &name) const;

	/**
	* Assigns a 3x3 matrix value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetMat3Value(const std::string &name, const mat3 &value);

	/**
	* Gets the value of a 3x3 matrix uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	mat3 GetMat3Value(const std::string &name) const;

	/**
	* Assigns a 4x4 matrix value to a named uniform variable.
	* @param name The name of the uniform variable.
	* @param value The value to assign.
	*/
	void SetMat4Value(const std::string &name, const mat4 &value);

	/**
	* Gets the value of a 4x4 matrix uniform variable.
	* @param name The name of the uniform variable.
	* @return The value of the uniform variable.
	*/
	mat4 GetMat4Value(const std::string &name) const;

	/**
	* Removes a variable from the list.
	* @param name The name of the variable to remove.
	*/
	void RemoveValue(const std::string &name);

	/**
	* Applies the list to a gpu program.
	* @param program The program to apply the list to.
	*/
	void ApplyForProgram(ProgramPtr program) const;

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
#pragma warning(pop)
