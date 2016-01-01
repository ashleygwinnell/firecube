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


std::string AssetUtils::ToString(vec4 v)
{
	std::ostringstream str;
	str << v.x << " " << v.y << " " << v.z << " " << v.w;
	return str.str();
}

AssetType AssetUtils::GetAssetTypeByPath(const std::string &path)
{
	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials", path))
	{
		return AssetType::MATERIAL;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scenes", path))
	{
		return AssetType::SCENE;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts", path))
	{
		return AssetType::SCRIPT;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Sounds", path))
	{
		return AssetType::SOUND;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Shaders", path))
	{
		return AssetType::SHADER;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Techniques", path))
	{
		return AssetType::TECHNIQUE;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Models", path))
	{
		return AssetType::MESH;
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures", path))
	{
		return AssetType::TEXTURE;
	}

	return AssetType::UNKNOWN;
}

std::string AssetUtils::ToString(vec3 v)
{
	std::ostringstream str;
	str << v.x << " " << v.y << " " << v.z;
	return str.str();
}

bool AssetUtils::SerializeMaterial(FireCube::Material *material, const std::string &path)
{
	TiXmlDocument doc;
	TiXmlElement *rootElement = new TiXmlElement("material");
	doc.LinkEndChild(rootElement);
	rootElement->SetAttribute("name", material->GetName());
	
	TiXmlElement *element;
	
	if (material->GetTechnique())
	{
		element = new TiXmlElement("technique");
		element->SetAttribute("name", material->GetTechnique()->GetFileName());
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_MATERIAL_DIFFUSE))
	{
		vec3 color = material->GetParameter(PARAM_MATERIAL_DIFFUSE).GetVec3();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialDiffuse");
		element->SetAttribute("value", ToString(color));
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_MATERIAL_SPECULAR))
	{
		vec3 color = material->GetParameter(PARAM_MATERIAL_SPECULAR).GetVec3();
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

	if (material->HasParameter(PARAM_MATERIAL_OPACITY))
	{
		float value = material->GetParameter(PARAM_MATERIAL_OPACITY).GetFloat();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "materialOpacity");
		element->SetDoubleAttribute("value", value);
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_U_OFFSET))
	{
		vec3 value = material->GetParameter(PARAM_U_OFFSET).GetVec3();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "uOffset");
		element->SetAttribute("value", ToString(value));
		rootElement->LinkEndChild(element);
	}

	if (material->HasParameter(PARAM_V_OFFSET))
	{
		vec3 value = material->GetParameter(PARAM_V_OFFSET).GetVec3();
		element = new TiXmlElement("parameter");
		element->SetAttribute("name", "vOffset");
		element->SetAttribute("value", ToString(value));
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
			element->SetAttribute("name", texture->GetFileName());
			rootElement->LinkEndChild(element);
		}
	}

	return doc.SaveFile(path);
}

std::string AssetUtils::ImportTextureIfNeeded(const std::string &texturePath)
{
	std::string sfile = texturePath;

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
	}
	else
	{
		Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures");

		std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
		Filesystem::CopyPath(sfile, targetPath);
		sfile = "Textures" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
	}

	std::replace(sfile.begin(), sfile.end(), '\\', '/');
	return sfile;
}

std::string AssetUtils::ImportTechniqueIfNeeded(const std::string &techniquePath)
{
	std::string sfile = techniquePath;

	if (Filesystem::IsSubPathOf(Filesystem::GetCoreDataFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetCoreDataFolder(), sfile);
	}
	else if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
	}
	else
	{
		Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Techniques");

		std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Techniques" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
		Filesystem::CopyPath(sfile, targetPath);
		sfile = "Techniques" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
	}
	std::replace(sfile.begin(), sfile.end(), '\\', '/');
	return sfile;
}

std::string AssetUtils::ImportMaterialIfNeeded(const std::string &materialPath)
{
	std::string sfile = materialPath;

	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), sfile))
	{
		sfile = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), sfile);
	}
	else
	{
		Filesystem::CreateFolder(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

		std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
		Filesystem::CopyPath(sfile, targetPath);
		sfile = "Materials" + Filesystem::PATH_SEPARATOR + Filesystem::GetLastPathComponent(sfile);
	}

	std::replace(sfile.begin(), sfile.end(), '\\', '/');
	return sfile;
}

char *AssetUtils::SerializeAssetDescription(AssetType type, const std::string &path, unsigned int &size)
{
	size = sizeof(AssetType) + path.size() + sizeof(unsigned int);
	char *ret = new char[size];
	unsigned int i = 0;
	*((AssetType *)&ret[i]) = type;
	i += sizeof(AssetType);
	*((unsigned int *)&ret[i]) = path.size();
	i += sizeof(unsigned int);
	for (unsigned int j = 0; j < path.size(); ++j)
	{
		ret[i++] = path[j];
	}

	return ret;
}

void AssetUtils::DeserializeAssetDescription(char *data, AssetType &type, std::string &path)
{
	unsigned int i = 0;
	type = *((AssetType *)&data[i]);
	i += sizeof(AssetType);
	unsigned int count = *((unsigned int *)&data[i]);
	i += sizeof(unsigned int);
	path.resize(count);
	for (unsigned int j = 0; j < count; ++j)
	{
		path[j] = data[i++];
	}
}