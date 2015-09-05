#include "Rendering/Material.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Rendering/Technique.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/Texture.h"

using namespace FireCube;
Material::Material(Engine *engine) : Resource(engine)
{
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
		textures[i] = nullptr;
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

			parameters[StringHash(parameterName)] = Variant::FromString(parameterValue);
		}
		else if (element->ValueStr() == "texture")
		{			
			std::string textureUnitName = element->Attribute("unit");
			if (textureUnitName.empty())
				continue;			
			std::string textureName = element->Attribute("name");
			if (textureName.empty())
				continue;		

			TextureUnit textureUnit = ParseTextureUnitName(textureUnitName);
			textures[static_cast<int>(textureUnit)] = engine->GetResourceCache()->GetResource<Texture>(textureName);
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

void Material::SetParameter(const StringHash &nameHash, int value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, float value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, bool value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, const vec2 &value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, const vec3 &value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, const vec4 &value)
{
	parameters[nameHash] = value;
}

void Material::SetParameter(const StringHash &nameHash, const Variant &value)
{
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

	return ret;
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