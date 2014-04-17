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

	void SetParameter(const StringHash &nameHash, int value);
	void SetParameter(const StringHash &nameHash, float value);
	void SetParameter(const StringHash &nameHash, bool value);
	void SetParameter(const StringHash &nameHash, const vec2 &value);
	void SetParameter(const StringHash &nameHash, const vec3 &value);
	void SetParameter(const StringHash &nameHash, const vec4 &value);
	void SetParameter(const StringHash &nameHash, const Variant &value);

	Texture **GetTextures();

	void SetTexture(TextureUnit textureUnit, Texture *texture);

	Texture *GetTexture(TextureUnit textureUnit) const;

	/**
	* Clones this Material.
	* @return A new cloned Material of this one.
	*/
	Material *Clone();
	
	static TextureUnit ParseTextureUnitName(const std::string &name);
private:
	
	
	std::string name;	
	Texture *textures[MAX_TEXTURE_UNITS];	
	Technique *technique;
	std::map<StringHash, Variant> parameters;
};

}
#pragma warning(pop)
