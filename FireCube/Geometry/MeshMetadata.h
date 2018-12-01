#pragma once

#include <string>
#include <unordered_map>

class TiXmlDocument;

namespace FireCube
{
	class MeshMetadata
	{
	public:
		MeshMetadata();
		MeshMetadata(const std::string &filename);
		void Load(const std::string &filename);
		void Save(const std::string &filename) const;
		std::string GetMaterialMapping(const std::string &name);
		bool HasMaterialMapping(const std::string &name);
		void RemoveMaterialMapping(const std::string &name);
		void SetMaterialMapping(const std::string &name, const std::string &target);
	private:
		void ReadMaterialMapping(TiXmlDocument &doc);
		std::unordered_map<std::string, std::string> materialMapping;
	};
}