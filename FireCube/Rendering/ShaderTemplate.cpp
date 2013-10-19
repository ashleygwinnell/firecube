#include <fstream>
#include <sstream>
#include "Rendering/ShaderTemplate.h"
#include "Utils/Filesystem.h"
#include "Rendering/Shader.h"
#include "Core/Engine.h"
using namespace FireCube;

static const char *shaderTemplateDefines[STP_MAX_PROPERTIES] = {"AMBIENT", 
																"PER_PIXEL_LIGHTING", 
																"POINT_LIGHT", 
																"DIRECTIONAL_LIGHT",
																"DIFFUSE_MAPPING", 
																"FOG", 
																"NORMAL_MAPPING",
																"SPOT_LIGHT"};

std::map<std::string, unsigned int> ShaderTemplate::strToShaderProperty;
bool ShaderTemplate::strToShaderPropertyInit = false;

ShaderTemplate::ShaderTemplate(Engine *engine) : Resource(engine)
{

}


bool ShaderTemplate::Load(const std::string &filename)
{
	std::string name = Filesystem::SearchForFileName(filename);
	if (name.empty())
		return false;	
	std::string::size_type d;
	d = filename.find_last_of(".");
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		if (ext == "vert")	
			type = VERTEX_SHADER;				
		else if (ext == "frag")
			type = FRAGMENT_SHADER;
		else
			return false;
	}
	else
		return false;

	std::ifstream f(name.c_str());
	if (!f.is_open())
		return false;

	shaderCode = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return true;
}

bool ShaderTemplate::Load(ShaderType type, const std::string &source)
{
	this->type = type;
	shaderCode = source;

	return true;
}

ShaderPtr ShaderTemplate::GenerateShader(unsigned int shaderProperties)
{
	std::map<unsigned int, ShaderPtr>::iterator i = shaders.find(shaderProperties);
	if (i != shaders.end())
		return i->second;

	// Add defines to the shader source according to the shader properties
	std::ostringstream defines;
	for (unsigned int i = 0; i < STP_MAX_PROPERTIES; ++i)
	{
		if (shaderProperties & (1 << i))
			defines << "#define " << shaderTemplateDefines[i] << std::endl;
	}
	std::ostringstream shaderSource;
	ShaderPtr shader(new Shader(engine->GetRenderer()));
	shaderSource << defines.str() << shaderCode;		
	shader->Create(type, shaderSource.str());		
	shaders[shaderProperties] = shader;
	return shader;
}

unsigned int ShaderTemplate::stringToShaderProperty(const std::string &property)
{
	if (!strToShaderPropertyInit)
	{
		strToShaderPropertyInit = true;
	
		strToShaderProperty["AMBIENT"] = STP_AMBIENT;
		strToShaderProperty["PER_PIXEL_LIGHTING"] = STP_PER_PIXEL_LIGHTING;
		strToShaderProperty["POINT_LIGHT"] = STP_POINT_LIGHT;
		strToShaderProperty["DIRECTIONAL_LIGHT"] = STP_DIRECTIONAL_LIGHT;
		strToShaderProperty["DIFFUSE_MAPPING"] = STP_DIFFUSE_MAPPING;
		strToShaderProperty["FOG"] = STP_FOG;
		strToShaderProperty["NORMAL_MAPPING"] = STP_NORMAL_MAPPING;
		strToShaderProperty["SPOT_LIGHT"] = STP_SPOT_LIGHT;
	}
	auto i = strToShaderProperty.find(property);
	if (i == strToShaderProperty.end())
		return 0;
	else
		return i->second;
}