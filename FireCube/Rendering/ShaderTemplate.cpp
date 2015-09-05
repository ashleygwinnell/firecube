#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "Rendering/ShaderTemplate.h"
#include "Utils/Filesystem.h"
#include "Rendering/Shader.h"
#include "Core/Engine.h"
using namespace FireCube;


ShaderTemplate::ShaderTemplate(Engine *engine) : Resource(engine)
{

}

ShaderTemplate::~ShaderTemplate()
{
	for (auto i : shaders)
		delete i.second;
}

bool ShaderTemplate::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;
	
	std::string::size_type d;
	d = filename.find_last_of(".");
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		if (ext == "vert")
			type = ShaderType::VERTEX_SHADER;
		else if (ext == "frag")
			type = ShaderType::FRAGMENT_SHADER;
		else
			return false;
	}
	else
		return false;

	std::ifstream f(resolvedFileName.c_str());
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

Shader *ShaderTemplate::GenerateShader(const std::string &defines)
{	
	std::stringstream ss(defines);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> definesList(begin, end);	
	std::sort(definesList.begin(), definesList.end());
	std::string sortedDefines = std::accumulate(definesList.begin(), definesList.end(), std::string(""));
	StringHash hash(sortedDefines);

	std::map<StringHash, Shader *>::iterator i = shaders.find(hash);
	if (i != shaders.end())
		return i->second;

	// Add defines to the shader source according to the shader properties
	std::ostringstream definesPrefix;
	definesPrefix << "#version 330" << std::endl;
	for (unsigned int i = 0; i < definesList.size(); ++i)
	{		
		definesPrefix << "#define " << definesList[i] << std::endl;
	}
	std::ostringstream shaderSource;
	Shader *shader = new Shader(engine->GetRenderer());
	shaderSource << definesPrefix.str() << shaderCode;
	shader->Create(type, shaderSource.str());
	shaders[hash] = shader;
	return shader;
}