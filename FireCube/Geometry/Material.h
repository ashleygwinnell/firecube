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
typedef std::shared_ptr<Texture> TexturePtr;
class Material;
class Technique;
typedef std::shared_ptr<Technique> TechniquePtr;
class Renderer;

/**
* A shared pointer to a Material.
*/
typedef std::shared_ptr<Material> MaterialPtr;

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

	void SetTechnique(TechniquePtr technique);

	TechniquePtr GetTechnique();

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

	const TexturePtr *GetTextures() const;

	void SetTexture(TextureUnit textureUnit, TexturePtr texture);

	TexturePtr GetTexture(TextureUnit textureUnit) const;

	/**
	* Clones this Material.
	* @return A new cloned Material of this one.
	*/
	MaterialPtr Clone();

private:

	static TextureUnit ParseTextureUnitName(const std::string &name);
	
	std::string name;	
	TexturePtr textures[MAX_TEXTURE_UNITS];	
	TechniquePtr technique;
	std::map<StringHash, Variant> parameters;
};

}
#pragma warning(pop)
