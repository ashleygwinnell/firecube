#pragma once

#include <FireCube.h>
#include <string>

namespace AssetUtils
{
	bool ImportMesh(FireCube::Engine *engine, const std::string &path);
	bool SerializeMaterial(FireCube::Material *material, const std::string &path);
}