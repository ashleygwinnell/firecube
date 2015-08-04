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
class Shader;
class Renderer;

class FIRECUBE_API Pass
{
public:	
	void SetName(const std::string &name);
	std::string GetName() const;
	void SetShaderDefines(const std::string &shaderDefines);
	void SetVertexShaderTemplate(ShaderTemplate *vertexShaderTemplate);
	void SetFragmentShaderTemplate(ShaderTemplate *fragmentShaderTemplate);
	Shader *GenerateVertexShader(const std::string &shaderDefines);
	Shader *GenerateFragmentShader(const std::string &shaderDefines);
	Shader *GetGeneratedVertexShader(unsigned int index);
	Shader *GetGeneratedFragmentShader(unsigned int index);
	void GenerateAllShaderPermutations();
	void SetBlendMode(BlendMode blendMode);
	BlendMode GetBlendMode() const;
	void SetIsBase(bool isBase);
	void SetDepthWrite(bool depthWrite);
	bool GetDepthWrite() const;
	void SetDepthTest(DepthTest depthTest);
	DepthTest GetDepthTest() const;
private:
	std::string name;
	ShaderTemplate *vertexShaderTemplate;
	ShaderTemplate *fragmentShaderTemplate;
	std::string shaderDefines;
	std::vector<Shader *> generatedVertexShaders;
	std::vector<Shader *> generatedFragmentShaders;
	BlendMode blendMode;
	bool isBase;
	bool depthWrite;
	DepthTest depthTest;
};

class FIRECUBE_API Technique : public Resource
{
	OBJECT(Technique)
public:
	Technique(Engine *engine);
	~Technique();

	Pass *GetPass(const StringHash &nameHash);
	bool Load(const std::string &filename);	
private:
	static BlendMode GetBlendModeFromString(const std::string &str);
	static DepthTest GetDepthTestFromString(const std::string &str);
	std::map<StringHash, Pass *> passes;		
};

}