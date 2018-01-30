#pragma once

#include <FireCube.h>
#include <string>

enum class AssetType
{
	MATERIAL, TEXTURE, SHADER, TECHNIQUE, MESH, SOUND, SCRIPT, SCENE, PREFAB, UNKNOWN
};

namespace AssetUtils
{
	bool ImportMesh(FireCube::Engine *engine, const std::string &path);
	bool SerializeMaterial(FireCube::Material *material, const std::string &path);
	std::string ImportTextureIfNeeded(const std::string &texturePath);	
	std::string ImportTechniqueIfNeeded(const std::string &techniquePath);
	std::string ImportMaterialIfNeeded(const std::string &materialPath);
	std::string ToString(FireCube::vec2 v);
	std::string ToString(FireCube::vec3 v);
	std::string ToString(FireCube::vec4 v);
	AssetType GetAssetTypeByPath(const std::string &path);
	char *SerializeAssetDescription(AssetType type, const std::string &path, unsigned int &size);	
	void DeserializeAssetDescription(const char *data, AssetType &type, std::string &path);
}