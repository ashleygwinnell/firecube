#include "Technique.h"
#include "ShaderTemplate.h"
#include "Rendering/Renderer.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Core/Engine.h"
#include "Core/ResourcePool.h"

using namespace FireCube;

static std::string shaderPermutationsDefines[] = { " SPOT_LIGHT",
									   " POINT_LIGHT",
									   " DIRECTIONAL_LIGHT"};
static std::string fogPermutationsDefines[] = { "",
									" FOG" };

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

	if (name == "base")
	{
		generatedVertexShaders.resize(2);
		generatedFragmentShaders.resize(2);

		for (unsigned int i = 0; i < 2; ++i)
		{			
			unsigned int fp = i;
			generatedVertexShaders[i] = vertexShaderTemplate->GenerateShader(shaderDefines + fogPermutationsDefines[fp]);
			generatedFragmentShaders[i] = fragmentShaderTemplate->GenerateShader(shaderDefines + fogPermutationsDefines[fp]);
		}
	}
	else
	{
		generatedVertexShaders.resize(MAX_SHADER_PERMUTATIONS * 2);
		generatedFragmentShaders.resize(MAX_SHADER_PERMUTATIONS * 2);

		for (unsigned int i = 0; i < MAX_SHADER_PERMUTATIONS * 2; ++i)
		{
			unsigned int sp = i % MAX_SHADER_PERMUTATIONS;
			unsigned int fp = i / MAX_SHADER_PERMUTATIONS;
			generatedVertexShaders[i] = vertexShaderTemplate->GenerateShader(shaderDefines + shaderPermutationsDefines[sp] + fogPermutationsDefines[fp]);
			generatedFragmentShaders[i] = fragmentShaderTemplate->GenerateShader(shaderDefines + shaderPermutationsDefines[sp] + fogPermutationsDefines[fp]);
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
			std::string shaderDefines = element->Attribute("defines");
			if (passName.empty() || vertexShaderTemplate.empty() || fragmentShaderTemplate.empty())
				continue;			
			Pass *pass = new Pass();
			pass->SetName(passName);
			pass->SetVertexShaderTemplate(engine->GetResourcePool()->GetResource<ShaderTemplate>(vertexShaderTemplate));
			pass->SetFragmentShaderTemplate(engine->GetResourcePool()->GetResource<ShaderTemplate>(fragmentShaderTemplate));
			pass->SetShaderDefines(shaderDefines);
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