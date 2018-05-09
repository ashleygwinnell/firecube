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

enum class MaterialParameterType
{
	NONE, FLOAT, VEC2, VEC3, VEC4, RGB, RGBA, INT, BOOL
};

/**
* A class representing a material parameter.
*/
class FIRECUBE_API MaterialParameter
{
public:
	std::string name;
	MaterialParameterType type;
	Variant value;
};

/**
* A class representing a material.
*/
class FIRECUBE_API Material : public Resource
{
	FIRECUBE_OBJECT(Material)
public:
	Material(Engine *engine);	

	/**
	* Loads a material for file
	* @param filename A path to the material file
	*/
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

	/**
	* Sets the technique used for this material
	* @param technique The technique to use
	*/
	void SetTechnique(Technique *technique);

	/**
	* @returns The technique used by this material 
	*/
	Technique *GetTechnique();
	
	/**
	* @returns All the shader parameters stored in this material
	*/
	const std::map<StringHash, MaterialParameter> &GetParameters() const;
	
	/**
	* @returns All the shader parameters stored in this material
	*/	
	std::map<StringHash, MaterialParameter> &GetParameters();

	/**
	* Checks whether the material has a given parameter
	* @param nameHash The hash of the parameter name
	*/
	bool HasParameter(const StringHash &nameHash) const;
	
	/**
	* Returns the parameter info of a give parameter
	* @param nameHash The hash of the parameter name
	*/
	MaterialParameter &GetParameter(const StringHash &nameHash);

	/**
	* Returns the value of a give parameter
	* @param nameHash The hash of the parameter name
	*/
	Variant &GetParameterValue(const StringHash &nameHash);

	/**
	* Sets an int parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, int value);
	
	/**
	* Sets a float parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, float value);
	
	/**
	* Sets a boolean parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, bool value);
	
	/**
	* Sets a vec2 parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, const vec2 &value);
	
	/**
	* Sets a vec3 parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, const vec3 &value);
	
	/**
	* Sets a vec4 parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, const vec4 &value);
	
	/**
	* Sets a parameter from a Variant
	* @param name The name of the parameter
	* @param type The type of the parameter
	* @value The value to set
	*/
	void SetParameter(const std::string &name, MaterialParameterType type, const Variant &value);

	/**
	* Sets an rgb parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetRGBParameter(const std::string &name, const vec3 &value);

	/**
	* Sets an rgba parameter
	* @param name The name of the parameter
	* @value The value to set
	*/
	void SetRGBAParameter(const std::string &name, const vec4 &value);

	/**
	* Removes a parameter
	* @param name The name of the parameter
	*/
	void RemoveParameter(const std::string &name);

	/**
	* @returns the textures used by this material
	*/
	Texture **GetTextures();

	/**
	* Sets a texture to be used by the material
	* @param textureUnit The texture unit to set (diffuse, normal, ...)
	* @param texture The texture to use
	*/
	void SetTexture(TextureUnit textureUnit, Texture *texture);

	/**
	* Returns a texture used by the material
	* @param textureUnit The texture unit to get (diffuse, normal, ...)
	*/
	Texture *GetTexture(TextureUnit textureUnit) const;

	/**
	* Clones this Material.
	* @return A new cloned Material of this one.
	*/
	Material *Clone();

	/**
	* Sets the culling mode of this material
	* @param cullMode The culling mode
	*/
	void SetCullMode(CullMode cullMode);
	
	
	/**
	* @returns The culling mode of this material
	*/
	CullMode GetCullMode() const;
	
	/**
	* @returns The name of a parameter given its hash
	*/
	std::string GetParameterName(StringHash nameHash) const;
	
	/**
	* Returns a texture unit from string
	* @param name The name of the texture unit 
	*/
	static TextureUnit ParseTextureUnitName(const std::string &name);
	
	/**
	* Gets the name of a texture unit 
	* @param textureUnit The texture unit
	*/
	static std::string GetTextureUnitName(TextureUnit unit);
	
	/**
	* Returns a cull mode from string
	* @param cullMode The name of the culling mode
	*/
	static CullMode ParseCullMode(const std::string &cullMode);

	static MaterialParameterType ParseParameterType(const std::string &type);
	static std::string ParameterTypeToString(const MaterialParameterType &type);
private:
		
	std::string name;	
	Texture *textures[static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS)];
	Technique *technique;
	CullMode cullMode;
	std::map<StringHash, MaterialParameter> parameters;
};

}
#pragma warning(pop)
