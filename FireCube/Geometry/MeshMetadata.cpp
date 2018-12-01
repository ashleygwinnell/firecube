#include "MeshMetadata.h"
#include "tinyxml.h"

using namespace FireCube;

MeshMetadata::MeshMetadata()
{

}

MeshMetadata::MeshMetadata(const std::string &filename)
{
	materialMapping.clear();
	Load(filename);
}

void MeshMetadata::Load(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(filename))
		return;

	ReadMaterialMapping(xmlDocument);
}

void MeshMetadata::ReadMaterialMapping(TiXmlDocument &doc)
{
	auto mappingElement = doc.FirstChildElement("material_mapping");
	if (!mappingElement)
	{
		return;
	}
	
	for (TiXmlElement *mapping = mappingElement->FirstChildElement(); mapping != nullptr; mapping = mapping->NextSiblingElement())
	{
		std::string name = mapping->Attribute("name");
		std::string target = mapping->Attribute("target");

		materialMapping[name] = target;
	}
}

std::string MeshMetadata::GetMaterialMapping(const std::string &name)
{
	auto i = materialMapping.find(name);
	if (i == materialMapping.end())
	{
		return "";
	}

	return i->second;
}

bool MeshMetadata::HasMaterialMapping(const std::string &name)
{
	return materialMapping.find(name) != materialMapping.end();
}

void MeshMetadata::Save(const std::string &filename) const
{
	TiXmlDocument doc;

	TiXmlElement *materialMappingElement = new TiXmlElement("material_mapping");
	doc.LinkEndChild(materialMappingElement);

	for (auto mapping : materialMapping)
	{
		TiXmlElement *element = new TiXmlElement("mapping");
		element->SetAttribute("name", mapping.first);
		element->SetAttribute("target", mapping.second);
		materialMappingElement->LinkEndChild(element);
	}

	doc.SaveFile(filename);
}

void MeshMetadata::RemoveMaterialMapping(const std::string &name)
{
	materialMapping.erase(name);
}

void MeshMetadata::SetMaterialMapping(const std::string &name, const std::string &target)
{
	materialMapping[name] = target;
}
