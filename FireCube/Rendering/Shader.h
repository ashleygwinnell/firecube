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
#include "Math/Math.h"
#include "Utils/StringHash.h"
#include "Core/Variant.h"

namespace FireCube
{

// Forward declarations.
class VertexBuffer;
class Program;
class Renderer;

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

	/**
	* Sets the names of output variables (used for shaders when transform feedback is enabled)
	*/
	void SetOutputAttributes(const std::vector<std::string> &output);

	/**	
	*@returns The output variables of this shader
	*/
	const std::vector<std::string> &GetOutputAttributes() const;
private:
	std::vector<std::string> outputVariables;	
};

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
	* Sets an array of 4x4 uniform matrix.
	* @param name The name of the variable.
	* @param value The array of matrices.
	* @param count The number of matrices
	*/
	void SetUniform(const StringHash &nameHash, mat4 *value, unsigned int count);

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
	* Sets a a from a variant.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void SetUniform(const StringHash &nameHash, const Variant &value);

	/**
	* @return Returns the compile log for this program.
	*/
	std::string GetInfoLog() const;

private:    
	std::map<StringHash, GLint> variables;	
};

}
#pragma warning(pop)
