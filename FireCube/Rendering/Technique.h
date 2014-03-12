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
	void SetShaderDefines(const std::string &shaderDefines);
	void SetVertexShaderTemplate(ShaderTemplatePtr vertexShaderTemplate);
	void SetFragmentShaderTemplate(ShaderTemplatePtr fragmentShaderTemplate);
	ShaderPtr GenerateVertexShader(const std::string &shaderDefines);
	ShaderPtr GenerateFragmentShader(const std::string &shaderDefines);
	ShaderPtr GetGeneratedVertexShader(unsigned int index);
	ShaderPtr GetGeneratedFragmentShader(unsigned int index);
	void GenerateAllShaderPermutations();
private:
	std::string name;
	ShaderTemplatePtr vertexShaderTemplate;
	ShaderTemplatePtr fragmentShaderTemplate;
	std::string shaderDefines;
	std::vector<ShaderPtr> generatedVertexShaders;
	std::vector<ShaderPtr> generatedFragmentShaders;
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