#ifndef MATERIAL_H
#define MATERIAL_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class Material;


/**
* A shared pointer to a Material.
*/
typedef std::shared_ptr<Material> MaterialPtr;

/**
* A class representing a material.
*/
class FIRECUBE_API Material
{
public:
	Material();
	~Material();


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
	* Gets the ambient color of this material.
	* @return The ambient color of this material.
	*/
	vec3 GetAmbientColor() const;

	/**
	* Sets the ambient color of this material.
	* @param color The color to set the ambient to.
	*/
	void SetAmbientColor(const vec3 &color);

	/**
	* Gets the diffuse color of this material.
	* @return The diffuse color of this material.
	*/
	vec3 GetDiffuseColor() const;

	/**
	* Sets the diffuse color of this material.
	* @param color The color to set the diffuse to.
	*/
	void SetDiffuseColor(const vec3 &color);

	/**
	* Gets the specular color of this material.
	* @return The specular color of this material.
	*/
	vec3 GetSpecularColor() const;

	/**
	* Sets the specular color of this material.
	* @param color The color to set the specular to.
	*/
	void SetSpecularColor(const vec3 &color);

	/**
	* Gets the shininess factor of this material.
	* @return The shininess factor of this material.
	*/
	float GetShininess() const;

	/**
	* Sets the shininess factor of this material.
	* @param value The shininess factor.
	*/
	void SetShininess(float value);

	/**
	* Gets the diffuse texture of this material.
	* @return The diffuse texture of this material.
	*/
	TexturePtr GetDiffuseTexture();

	/**
	* Sets the diffuse texture of this material.
	* @param texture The new diffuse texture map.
	*/
	void SetDiffuseTexture(TexturePtr texture);

	/**
	* Gets the normal texture of this material.
	* @return The normal texture of this material.
	*/
	TexturePtr GetNormalTexture();

	/**
	* Sets the normal texture of this material.
	* @param texture The new normal texture map.
	*/
	void SetNormalTexture(TexturePtr texture);

	/**
	* Gets the opacity of this material.
	* @return The opacity factor of this material.
	*/
	float GetOpacity() const;

	/**
	* Sets the opacity of this material.
	* @param value The opacity.
	*/
	void SetOpacity(float value);

	/**
	* Clones this Material.
	* @return A new cloned Material of this one.
	*/
	MaterialPtr Clone();

private:

	std::string name;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	TexturePtr diffuseTexture;
	TexturePtr normalTexture;
	float opacity;
};

}
#pragma warning(pop)

#endif