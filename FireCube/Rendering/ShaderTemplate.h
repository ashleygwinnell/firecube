#pragma once

#include <map>
#include "Utils/utils.h"
#include "Rendering/RenderingTypes.h"
#include "Core/Resource.h"

namespace FireCube
{

class Renderer;
class Shader;

/**
* This class stores a shader template. An actual shader is generated given a list of preprocessor defines.
*/
class FIRECUBE_API ShaderTemplate : public Resource
{
	FIRECUBE_OBJECT(ShaderTemplate)
public:
	ShaderTemplate(Engine *engine);
	~ShaderTemplate();
	
	/**
	* Loads the specified shader.
	* @param filename The file name of the shader.
	* @return True on success, false otherwise.
	*/
	bool Load(const std::string &filename);

	/**
	* Loads the specified shader.
	* @param type The type of the shader.
	* @param source The source of the shader.
	* @return True on success, false otherwise.
	*/
	bool Load(ShaderType type, const std::string &source);

		
	/**
	* Generates a shader from a list of preprocessor defines.
	* @param defines The shader preprocessor defines to use to generate the shader.
	*/	
	Shader *GenerateShader(const std::string &defines);	

private:	
	std::map<StringHash, std::pair<std::string, Shader *>> shaders;
	std::string shaderCode;		
	ShaderType type;
};

}