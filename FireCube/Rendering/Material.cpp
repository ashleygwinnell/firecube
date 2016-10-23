#include "Rendering/Material.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Rendering/Technique.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/Texture2D.h"
#include "Rendering/TextureCube.h"

using namespace FireCube;
Material::Material(Engine *engine) : Resource(engine), technique(nullptr), cullMode(CullMode::CCW)
{
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
		textures[i] = nullptr;

	SetParameter(PARAM_MATERIAL_U_OFFSET, vec3(1, 0, 0));
	SetParameter(PARAM_MATERIAL_V_OFFSET, vec3(0, 1, 0));
}

bool Material::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(resolvedFileName))
		return false;

	TiXmlElement *e = xmlDocument.FirstChildElement("material");
	if (e == nullptr)
		return false;
	this->filename = filename;

	if (e->Attribute("name"))
	{
		name = e->Attribute("name");
	}

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "technique")
		{			
			std::string techniqueName = element->Attribute("name");
			if (techniqueName.empty())
				continue;			
			
			technique = engine->GetResourceCache()->GetResource<Technique>(techniqueName);			
		}
		else if (element->ValueStr() == "parameter")
		{			
			std::string parameterName = element->Attribute("name");
			if (parameterName.empty())
				continue;			
			std::string parameterValue = element->Attribute("value");
			if (parameterValue.empty())
				continue;		

			SetParameter(parameterName, Variant::FromString(parameterValue));			
		}
		else if (element->ValueStr() == "texture")
		{			
			std::string textureUnitName = element->Attribute("unit");
			if (textureUnitName.empty())
				continue;			
			std::string textureName = element->Attribute("name");
			if (textureName.empty())
				continue;	

			std::string textureType = "2d";
			if (element->Attribute("type"))
			{
				textureType = element->Attribute("type");
			}

			TextureUnit textureUnit = ParseTextureUnitName(textureUnitName);
			if (textureType == "2d")
			{
				textures[static_cast<int>(textureUnit)] = engine->GetResourceCache()->GetResource<Texture2D>(textureName);
			}
			else if (textureType == "cube")
			{
				textures[static_cast<int>(textureUnit)] = engine->GetResourceCache()->GetResource<TextureCube>(textureName);
			}
		}
		else if (element->ValueStr() == "cull")
		{
			cullMode = ParseCullMode(element->Attribute("value"));
		}
	}
	return true;
}

std::string Material::GetName() const
{
	return name;
}

void Material::SetName(const std::string &name)
{
	this->name = name;
}

Material *Material::Clone()
{
	Material *ret = new Material(engine);	
	ret->name = name;
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
		ret->textures[i] = textures[i];
	ret->technique = technique;
	ret->parameters = parameters;	
	return ret;
}

void Material::SetTechnique(Technique *technique)
{
	this->technique = technique;
}

Technique *Material::GetTechnique()
{
	return technique;
}

std::map<StringHash, Variant> &Material::GetParameters()
{
	return parameters;
}

const std::map<StringHash, Variant> &Material::GetParameters() const
{
	return parameters;
}

void Material::SetParameter(const std::string &name, int value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, float value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, bool value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, const vec2 &value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, const vec3 &value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, const vec4 &value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

void Material::SetParameter(const std::string &name, const Variant &value)
{
	StringHash nameHash(name);
	parametersNames[nameHash] = name;
	parameters[nameHash] = value;
}

Texture **Material::GetTextures()
{
	return textures;
}

void Material::SetTexture(TextureUnit textureUnit, Texture *texture)
{
	if (static_cast<int>(textureUnit) < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS))
		textures[static_cast<int>(textureUnit)] = texture;
}

Texture *Material::GetTexture(TextureUnit textureUnit) const
{
	if (static_cast<int>(textureUnit) < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS))
		return textures[static_cast<int>(textureUnit)];
	else
		return nullptr;
}

TextureUnit Material::ParseTextureUnitName(const std::string &name)
{
	TextureUnit ret = TextureUnit::DIFFUSE;
	if (name == "diffuse")
		ret = TextureUnit::DIFFUSE;
	else if (name == "normal")
		ret = TextureUnit::NORMAL;
	else if (name == "specular")
		ret = TextureUnit::SPECULAR;

	return ret;
}

std::string Material::GetTextureUnitName(TextureUnit unit)
{
	switch (unit)
	{
	case FireCube::TextureUnit::DIFFUSE:
		return "diffuse";
		break;
	case FireCube::TextureUnit::NORMAL:
		return "normal";
		break;	
	case FireCube::TextureUnit::SPECULAR:
		return "specular";
		break;
	default:
		return "";
		break;
	}
}

bool Material::HasParameter(const StringHash &nameHash) const
{
	return parameters.find(nameHash) != parameters.end();
}

Variant &Material::GetParameter(const StringHash &nameHash)
{
	auto i = parameters.find(nameHash);
	return i->second;	
}

void Material::SetCullMode(CullMode cullMode)
{
	this->cullMode = cullMode;
}

CullMode Material::GetCullMode() const
{
	return cullMode;
}

std::string Material::GetParameterName(StringHash nameHash) const
{
	auto i = parametersNames.find(nameHash);
	if (i != parametersNames.end())
	{
		return i->second;
	}
	
	return "";
}

const std::map<StringHash, std::string> &Material::GetParametersNames() const
{
	return parametersNames;
}

void Material::RemoveParameter(const std::string &name)
{
	StringHash nameHash(name);
	parametersNames.erase(nameHash);
	parameters.erase(nameHash);
}

FireCube::CullMode FireCube::Material::ParseCullMode(const std::string &cullMode)
{
	CullMode ret = CullMode::CCW;
	if (cullMode == "none")
		ret = CullMode::NONE;
	else if (cullMode == "cw")
		ret = CullMode::CW;
	else if (cullMode == "ccw")
		ret = CullMode::CCW;

	return ret;
}
