#pragma once

#include <FireCube.h>
#include <string>

namespace AssetUtils
{
	bool ImportMesh(FireCube::Engine *engine, const std::string &path);
	bool SerializeMaterial(FireCube::Material *material, const std::string &path);
	std::string ImportTextureIfNeeded(const std::string &texturePath);	
	std::string ImportTechniqueIfNeeded(const std::string &techniquePath);
	std::string ImportMaterialIfNeeded(const std::string &materialPath);
}