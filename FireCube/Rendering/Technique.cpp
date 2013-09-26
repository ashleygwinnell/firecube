#include "Technique.h"
#include "ShaderTemplate.h"
#include "Rendering/Renderer.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Core/Engine.h"
#include "Core/ResourcePool.h"

using namespace FireCube;

void Pass::SetName(const std::string &name)
{
	this->name = name;
}

std::string Pass::GetName() const
{
	return name;
}

void Pass::SetShaderProperties(unsigned int shaderProperties)
{
	this->shaderProperties = shaderProperties;
}

void Pass::SetVertexShaderTemplate(ShaderTemplatePtr vertexShaderTemplate)
{
	this->vertexShaderTemplate = vertexShaderTemplate;
}

void Pass::SetFragmentShaderTemplate(ShaderTemplatePtr fragmentShaderTemplate)
{
	this->fragmentShaderTemplate = fragmentShaderTemplate;
}

ShaderPtr Pass::GenerateVertexShader(unsigned int shaderProperties)
{
	return vertexShaderTemplate->GenerateShader(this->shaderProperties | shaderProperties);
}

ShaderPtr Pass::GenerateFragmentShader(unsigned int shaderProperties)
{
	return fragmentShaderTemplate->GenerateShader(this->shaderProperties | shaderProperties);
}

Technique::Technique(Engine *engine) : Resource(engine)
{
	
}

bool Technique::Load(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(Filesystem::SearchForFileName(filename)))
		return false;
	
	TiXmlElement *e = xmlDocument.FirstChildElement("technique");
	if (e == nullptr)
		return false;

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "pass")
		{			
			std::string passName = element->Attribute("name");
			std::string vertexShaderTemplate = element->Attribute("vs");
			std::string fragmentShaderTemplate = element->Attribute("fs");
			std::string shaderPropertiesStr = element->Attribute("properties");
			if (passName.empty() || vertexShaderTemplate.empty() || fragmentShaderTemplate.empty())
				continue;			
			PassPtr pass(new Pass);
			unsigned int shaderProperties = 0;
			std::vector<std::string> properties = Split(shaderPropertiesStr, ' ');
			for (auto &propertyStr : properties)
			{
				propertyStr = Trim(propertyStr);
				unsigned int property = ShaderTemplate::stringToShaderProperty(propertyStr);
				shaderProperties |= property;
			}
			pass->SetName(passName);
			pass->SetVertexShaderTemplate(engine->GetResourcePool()->GetResource<ShaderTemplate>(vertexShaderTemplate));
			pass->SetFragmentShaderTemplate(engine->GetResourcePool()->GetResource<ShaderTemplate>(fragmentShaderTemplate));
			pass->SetShaderProperties(shaderProperties);
			passes[StringHash(passName)] = pass;
		}
	}
	return true;
}

Pass *Technique::GetPass(const StringHash &nameHash)
{
	auto i = passes.find(nameHash);
	if (i != passes.end())
		return i->second.get();
	return nullptr;
}