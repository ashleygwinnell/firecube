#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <map>
#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Resource.h"
#include "Core/Variant.h"
#include "Rendering/RenderingTypes.h"

namespace FireCube
{

// Forward declarations.
class Texture;
class Material;
class Technique;
class Renderer;

/**
* A class representing a material.
*/
class FIRECUBE_API Material : public Resource
{
	FIRECUBE_OBJECT(Material)
public:
	Material(Engine *engine);	

	bool Load(const std::string &filename);

	/**
	* Gets the name of this material.
	* @return The name of the material.
	*/
	std::string GetName() const;

	/**
	* Sets the name of the material.
	* @param name The name of the material.
	*/
	void SetName(const std::string &name);

	void SetTechnique(Technique *technique);

	Technique *GetTechnique();

	const std::map<StringHash, Variant> &GetParameters() const;	
	std::map<StringHash, Variant> &GetParameters();	

	bool HasParameter(const StringHash &nameHash) const;
	Variant &GetParameter(const StringHash &nameHash);

	void SetParameter(const std::string &name, int value);
	void SetParameter(const std::string &name, float value);
	void SetParameter(const std::string &name, bool value);
	void SetParameter(const std::string &name, const vec2 &value);
	void SetParameter(const std::string &name, const vec3 &value);
	void SetParameter(const std::string &name, const vec4 &value);
	void SetParameter(const std::string &name, const Variant &value);

	void RemoveParameter(const std::string &name);

	Texture **GetTextures();

	void SetTexture(TextureUnit textureUnit, Texture *texture);

	Texture *GetTexture(TextureUnit textureUnit) const;

	/**
	* Clones this Material.
	* @return A new cloned Material of this one.
	*/
	Material *Clone();

	void SetCullMode(CullMode cullMode);
	CullMode GetCullMode() const;
	
	const std::map<StringHash, std::string> &GetParametersNames() const;
	std::string Material::GetParameterName(StringHash nameHash) const;
	
	static TextureUnit ParseTextureUnitName(const std::string &name);
	static std::string GetTextureUnitName(TextureUnit unit);
	static CullMode ParseCullMode(const std::string &cullMode);
private:
		
	std::string name;	
	Texture *textures[static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS)];
	Technique *technique;
	CullMode cullMode;
	std::map<StringHash, Variant> parameters;
	std::map<StringHash, std::string> parametersNames;
};

}
#pragma warning(pop)
