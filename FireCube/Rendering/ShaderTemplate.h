#pragma once

#include <map>
#include "Utils/utils.h"
#include "Rendering/RenderingTypes.h"
#include "Core/Resource.h"

namespace FireCube
{

class Renderer;
class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

enum ShaderTemplateProperty
{
	STP_AMBIENT = 1,
	STP_PER_PIXEL_LIGHTING = 2,
	STP_POINT_LIGHT = 4,
	STP_DIRECTIONAL_LIGHT = 8,
	STP_DIFFUSE_MAPPING = 16,
	STP_FOG = 32,
	STP_NORMAL_MAPPING = 64,
	STP_SPOT_LIGHT = 128,
	STP_MAX_PROPERTIES = 8
};

class FIRECUBE_API ShaderTemplate : public Resource
{
public:
	ShaderTemplate(Engine *engine);
	
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
	* Generates a shader from a given shader properties.
	* @param shaderProperties The shader properties to use to generate the shader.
	*/
	ShaderPtr GenerateShader(unsigned int shaderProperties);

	static unsigned int stringToShaderProperty(const std::string &property);

private:
	static std::map<std::string, unsigned int> strToShaderProperty;
	static bool strToShaderPropertyInit;
	std::map<unsigned int, ShaderPtr> shaders;
	std::string shaderCode;		
	ShaderType type;
};

typedef std::shared_ptr<ShaderTemplate> ShaderTemplatePtr;

}