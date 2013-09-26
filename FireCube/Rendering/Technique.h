#pragma once

#include <memory>
#include <vector>
#include <map>
#include "Utils/utils.h"
#include "RenderingTypes.h"
#include "Core/Resource.h"

namespace FireCube
{

class ShaderTemplate;
typedef std::shared_ptr<ShaderTemplate> ShaderTemplatePtr;
class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;
class Renderer;

class FIRECUBE_API Pass
{
public:
	void SetName(const std::string &name);
	std::string GetName() const;
	void SetShaderProperties(unsigned int shaderProperties);
	void SetVertexShaderTemplate(ShaderTemplatePtr vertexShaderTemplate);
	void SetFragmentShaderTemplate(ShaderTemplatePtr fragmentShaderTemplate);
	ShaderPtr GenerateVertexShader(unsigned int shaderProperties);
	ShaderPtr GenerateFragmentShader(unsigned int shaderProperties);
private:
	std::string name;
	ShaderTemplatePtr vertexShaderTemplate;
	ShaderTemplatePtr fragmentShaderTemplate;
	unsigned int shaderProperties;
};

typedef std::shared_ptr<Pass> PassPtr;

class FIRECUBE_API Technique : public Resource
{
public:
	Technique(Engine *engine);
	Pass *GetPass(const StringHash &nameHash);
	bool Load(const std::string &filename);	
private:
	std::map<StringHash, PassPtr> passes;	
	Renderer *renderer;
};

/**
* A shared pointer to a Technique.
*/
typedef std::shared_ptr<Technique> TechniquePtr;

}