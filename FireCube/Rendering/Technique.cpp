#include "Technique.h"
#include "ShaderTemplate.h"
#include "Rendering/Renderer.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Core/Variant.h"

using namespace FireCube;

static std::string vertexShaderLightPermutationsDefines[] = { " SPOT_LIGHT",
															  " POINT_LIGHT",
															  " DIRECTIONAL_LIGHT"};
static std::string fragmentShaderLightPermutationsDefines[] = { " SPOT_LIGHT",
																" POINT_LIGHT",
																" DIRECTIONAL_LIGHT" };

static std::string fogPermutationsDefines[] = { "",
												" FOG" };

static std::string geometryPermutationsDefines[] = { "",
													 " SKINNING" };

void Pass::SetName(const std::string &name)
{
	this->name = name;
}

std::string Pass::GetName() const
{
	return name;
}

void Pass::SetShaderDefines(const std::string &shaderDefines)
{
	this->shaderDefines = shaderDefines;
}

void Pass::SetVertexShaderTemplate(ShaderTemplate *vertexShaderTemplate)
{
	this->vertexShaderTemplate = vertexShaderTemplate;
}

void Pass::SetFragmentShaderTemplate(ShaderTemplate *fragmentShaderTemplate)
{
	this->fragmentShaderTemplate = fragmentShaderTemplate;
}

Shader *Pass::GenerateVertexShader(const std::string &shaderDefines)
{
	return vertexShaderTemplate->GenerateShader(this->shaderDefines + " " + shaderDefines);
}

Shader *Pass::GenerateFragmentShader(const std::string &shaderDefines)
{
	return fragmentShaderTemplate->GenerateShader(this->shaderDefines + " " + shaderDefines);
}

void Pass::GenerateAllShaderPermutations()
{
	if (generatedVertexShaders.empty() == false && generatedFragmentShaders.empty() == false)
		return;

	if (isBase)
	{
		generatedVertexShaders.resize(2);
		generatedFragmentShaders.resize(2);		

		for (unsigned int i = 0; i < 2; ++i)
		{
			unsigned int g = i;
			generatedVertexShaders[i] = vertexShaderTemplate->GenerateShader(shaderDefines + geometryPermutationsDefines[g]);			
		}

		for (unsigned int i = 0; i < 2; ++i)
		{			
			unsigned int f = i;			
			generatedFragmentShaders[i] = fragmentShaderTemplate->GenerateShader(shaderDefines + fogPermutationsDefines[f]);
		}
	}
	else
	{
		generatedVertexShaders.resize(MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS * 2);
		generatedFragmentShaders.resize(MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS * 2);

		for (unsigned int i = 0; i < MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS * 2; ++i)
		{
			unsigned int l = i % MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS;			
			unsigned int g = i / MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS;
			generatedVertexShaders[i] = vertexShaderTemplate->GenerateShader(shaderDefines + vertexShaderLightPermutationsDefines[l] + geometryPermutationsDefines[g]);			
		}

		for (unsigned int i = 0; i < MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS * 2; ++i)
		{
			unsigned int l = i % MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS;			
			unsigned int f = i / MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS;
			generatedFragmentShaders[i] = fragmentShaderTemplate->GenerateShader(shaderDefines + fragmentShaderLightPermutationsDefines[l] + fogPermutationsDefines[f]);
		}
	}	
}

Shader *Pass::GetGeneratedVertexShader(unsigned int index)
{
	return generatedVertexShaders[index];
}

Shader *Pass::GetGeneratedFragmentShader(unsigned int index)
{
	return generatedFragmentShaders[index];
}

void Pass::SetBlendMode(BlendMode blendMode)
{
	this->blendMode = blendMode;
}

BlendMode Pass::GetBlendMode() const
{
	return blendMode;
}

void Pass::SetDepthWrite(bool depthWrite)
{
	this->depthWrite = depthWrite;
}

bool Pass::GetDepthWrite() const
{
	return depthWrite;
}

void Pass::SetDepthTest(DepthTest depthTest)
{
	this->depthTest = depthTest;
}

DepthTest Pass::GetDepthTest() const
{
	return depthTest;
}

Technique::Technique(Engine *engine) : Resource(engine)
{
	
}

Technique::~Technique()
{
	for (auto i : passes)
		delete i.second;
}

bool Technique::Load(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(Filesystem::SearchForFileName(filename)))
		return false;
	
	TiXmlElement *e = xmlDocument.FirstChildElement("technique");
	if (e == nullptr)
		return false;
	this->filename = filename;

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "pass")
		{			
			std::string passName = element->Attribute("name");
			std::string vertexShaderTemplate = element->Attribute("vs");
			std::string fragmentShaderTemplate = element->Attribute("fs");
			
			const char *shaderDefinesStr = element->Attribute("defines");
			std::string shaderDefines = shaderDefinesStr == nullptr ? "" : shaderDefinesStr;
			
			const char *blendStr = element->Attribute("blend");
			std::string blendMode = blendStr == nullptr ? "replace" : blendStr;

			const char *isBaseStr = element->Attribute("is_base");
			bool isBase = isBaseStr ? Variant::FromString(isBaseStr).GetBool() : (passName == "base" || passName == "alpha");

			const char *depthWriteStr = element->Attribute("depth_write");
			bool depthWrite = depthWriteStr ? Variant::FromString(depthWriteStr).GetBool() : true;

			const char *depthTestStr = element->Attribute("depth_test");
			std::string depthTest = depthTestStr == nullptr ? "less" : depthTestStr;
			
			if (passName.empty() || vertexShaderTemplate.empty() || fragmentShaderTemplate.empty())
				continue;			
			Pass *pass = new Pass();
			pass->SetName(passName);
			pass->SetVertexShaderTemplate(engine->GetResourceCache()->GetResource<ShaderTemplate>(vertexShaderTemplate));
			pass->SetFragmentShaderTemplate(engine->GetResourceCache()->GetResource<ShaderTemplate>(fragmentShaderTemplate));
			pass->SetShaderDefines(shaderDefines);
			pass->SetBlendMode(Technique::GetBlendModeFromString(blendMode));
			pass->SetIsBase(isBase);
			pass->SetDepthWrite(depthWrite);
			pass->SetDepthTest(Technique::GetDepthTestFromString(depthTest));
			passes[StringHash(passName)] = pass;
		}
	}
	return true;
}

Pass *Technique::GetPass(const StringHash &nameHash)
{
	auto i = passes.find(nameHash);
	if (i != passes.end())
		return i->second;
	return nullptr;
}

BlendMode Technique::GetBlendModeFromString(const std::string &str)
{
	if (str == "replace")
	{
		return BlendMode::REPLACE;
	}
	else if (str == "add")
	{
		return BlendMode::ADD;
	}

	return BlendMode::REPLACE;
}

DepthTest Technique::GetDepthTestFromString(const std::string &str)
{
	if (str == "always")
	{
		return DepthTest::ALWAYS;
	}
	else if (str == "never")
	{
		return DepthTest::NEVER;
	}
	else if (str == "equal")
	{
		return DepthTest::EQUAL;
	}
	else if (str == "less")
	{
		return DepthTest::LESS;
	}
	else if (str == "less_equal")
	{
		return DepthTest::LESSEQUAL;
	}

	return DepthTest::LESS;
}

void Pass::SetIsBase(bool isBase)
{
	this->isBase = isBase;
}