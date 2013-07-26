#include "Geometry/Material.h"

using namespace FireCube;
Material::Material() : shininess(128.0f), opacity(1.0f)
{

}

Material::~Material()
{

}

std::string Material::GetName() const
{
	return name;
}

void Material::SetName(const std::string &name)
{
	this->name = name;
}

vec3 Material::GetAmbientColor() const
{
	return ambient;
}

void Material::SetAmbientColor(const vec3 &color)
{
	ambient = color;
}

vec3 Material::GetDiffuseColor() const
{
	return diffuse;
}

void Material::SetDiffuseColor(const vec3 &color)
{
	diffuse = color;
}

vec3 Material::GetSpecularColor() const
{
	return specular;
}

void Material::SetSpecularColor(const vec3 &color)
{
	specular = color;
}

float Material::GetShininess() const
{
	return shininess;
}

void Material::SetShininess(float value)
{
	shininess = value;
}

TexturePtr Material::GetDiffuseTexture()
{
	return diffuseTexture;
}

void Material::SetDiffuseTexture(TexturePtr texture)
{
	diffuseTexture = texture;
}

TexturePtr Material::GetNormalTexture()
{
	return normalTexture;
}

void Material::SetNormalTexture(TexturePtr texture)
{
	normalTexture = texture;
}

float Material::GetOpacity() const
{
	return opacity;
}

void Material::SetOpacity(float value)
{
	opacity = value;
}

MaterialPtr Material::Clone()
{
	MaterialPtr ret(new Material);
	ret->ambient = ambient;
	ret->diffuse = diffuse;
	ret->diffuseTexture = diffuseTexture;
	ret->name = name;
	ret->normalTexture = normalTexture;
	ret->opacity = opacity;
	ret->shininess = shininess;
	ret->specular = specular;
	return ret;
}