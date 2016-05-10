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

/**
* This class represents a rendering pass in a technique
*/
class FIRECUBE_API Pass
{
public:	

	/**
	* Sets the name of the pass
	* @param name The name of the pass
	*/
	void SetName(const std::string &name);

	/**
	* @returns The name of the pass
	*/
	std::string GetName() const;

	/**
	* Sets the shader defines to use when generating shaders from the shader template
	* @param shaderDefines A list of defines separated by space
	*/
	void SetShaderDefines(const std::string &shaderDefines);

	/**
	* Sets the shader template for the vertex shader used in this pass
	* @param vertexShaderTemplate The shader template to use
	*/
	void SetVertexShaderTemplate(ShaderTemplate *vertexShaderTemplate);

	/**
	* Sets the shader template for the fragment shader used in this pass
	* @param fragmentShaderTemplate The shader template to use
	*/
	void SetFragmentShaderTemplate(ShaderTemplate *fragmentShaderTemplate);

	Shader *GenerateVertexShader(const std::string &shaderDefines);
	Shader *GenerateFragmentShader(const std::string &shaderDefines);

	/**
	* Returns a vertex shader for a given shader permutation
	* @param index The shader permutation to get
	*/
	Shader *GetGeneratedVertexShader(unsigned int index);

	/**
	* Returns a fragment shader for a given shader permutation
	* @param index The shader permutation to get
	*/
	Shader *GetGeneratedFragmentShader(unsigned int index);

	/**
	* Generates all vertex and fragment shaders permutations
	*/
	void GenerateAllShaderPermutations();

	/**
	* Sets the blending mode
	* @param blendMode The blending mode to set
	*/
	void SetBlendMode(BlendMode blendMode);

	/**
	* @returns The blending mode used in this pass
	*/
	BlendMode GetBlendMode() const;

	/**
	* Sets whether this pass is a base pass. Base passes have fewer shader permutations (since they don't perform lighting)
	* @param isBase Whether this pass is a base pass
	*/
	void SetIsBase(bool isBase);

	/**
	* Sets whether depth writing should be enabled for this pass
	* @param depthWrite Whether to enable depth writing
	*/
	void SetDepthWrite(bool depthWrite);

	/**
	* @returns Whether depth writing is enabled for this pass
	*/
	bool GetDepthWrite() const;

	/**
	* Sets the depth testing mode for this pass
	* @param depthTest The depth testing mode to set
	*/
	void SetDepthTest(DepthTest depthTest);

	/**
	* @returns The depth testing mode of this pass
	*/
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
/**
* This pass represnets a technique which is a collection of passes identified by name
*/
class FIRECUBE_API Technique : public Resource
{
	FIRECUBE_OBJECT(Technique)
public:
	Technique(Engine *engine);
	~Technique();

	/**
	* Returns a pass given its name hash
	* @param nameHash The name hash of the pass
	*/
	Pass *GetPass(const StringHash &nameHash);

	/**
	* Loads a technique from an xml file
	* @param filename The path to load
	*/
	bool Load(const std::string &filename);	
private:
	static BlendMode GetBlendModeFromString(const std::string &str);
	static DepthTest GetDepthTestFromString(const std::string &str);
	std::map<StringHash, Pass *> passes;		
};

}