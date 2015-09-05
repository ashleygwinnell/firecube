#include "tinyxml.h"
#include "AssetUtils.h"
using namespace FireCube;

bool AssetUtils::ImportMesh(FireCube::Engine *engine, const std::string &path)
{
	std::string filename = Filesystem::GetLastPathComponent(path);
	std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Models" + Filesystem::PATH_SEPARATOR + filename;

	if (!Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Models"))
		return false;

	if (!Filesystem::CopyPath(path, targetPath))
		return false;

	Mesh *mesh = new Mesh(engine);
	if (!mesh->Load(path))
		return false;

	auto materials = mesh->GetMaterials();

	std::set<SharedPtr<Material>> materialsSet(materials.begin(), materials.end());	

	if (!Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials"))
		return false;
	if (!Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures"))
		return false;

	for (auto material : materialsSet)
	{
		std::string targetMaterialPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials" + Filesystem::PATH_SEPARATOR + material->GetName() + ".xml";
		SerializeMaterial(material, targetMaterialPath);

		for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
		{
			TextureUnit unit = static_cast<TextureUnit>(i);
			Texture *texture = material->GetTexture(unit);

			if (texture)
			{
				std::string targetTexturePath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(texture->GetFileName());
				Filesystem::CopyPath(texture->GetFileName(), targetTexturePath);
			}
		}
	}
	
	delete mesh;
	return true;
}


std::string ToString(vec4 v)
{
	std::ostringstream str;
	str << v.x << " " << v.y << " " << v.z << " " << v.w;
	return str.str();
}

bool AssetUtils::SerializeMaterial(FireCube::Material *material, const std::string &path)
{
	TiXmlDocument doc;
	TiXmlElement *rootElement = new TiXmlElement("material");
	doc.LinkEndChild(rootElement);
	rootElement->SetAttribute("name", material->GetName());
	
	TiXmlElement *element;
	
	element = new TiXmlElement("technique");
	element->SetAttribute("name", material->GetTechnique()->GetFileName());
	rootElement->LinkEndChild(element);

	if (material->HasParameter(PARAM_MATERIAL_AMBIENT))
	{
		vec4 color = material->GetParameter(PARAM_MATERIAL_AMBIENT).GetVec4();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialAmbient");
		element->SetAttribute("value", ToString(color));
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_MATERIAL_DIFFUSE))
	{
		vec4 color = material->GetParameter(PARAM_MATERIAL_DIFFUSE).GetVec4();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialDiffuse");
		element->SetAttribute("value", ToString(color));
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_MATERIAL_SPECULAR))
	{
		vec4 color = material->GetParameter(PARAM_MATERIAL_SPECULAR).GetVec4();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialSpecular");
		element->SetAttribute("value", ToString(color));
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_MATERIAL_SHININESS))
	{
		float value = material->GetParameter(PARAM_MATERIAL_SHININESS).GetFloat();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialShininess");
		element->SetDoubleAttribute("value", value);
		rootElement->LinkEndChild(element);
	}

	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
	{
		TextureUnit unit = static_cast<TextureUnit>(i);
		Texture *texture = material->GetTexture(unit);

		if (texture)
		{			
			element = new TiXmlElement("texture");
			element->SetAttribute("unit", Material::GetTextureUnitName(unit));
			element->SetAttribute("name", "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(texture->GetFileName()));
			rootElement->LinkEndChild(element);
		}
	}

	return doc.SaveFile(path);
}