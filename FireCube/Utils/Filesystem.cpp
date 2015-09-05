#include "Utils/Logger.h"
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include "Utils/Filesystem.h"

using namespace FireCube;

std::string assetsFolder = "";
std::string coreDataFolder = "";

std::string Filesystem::GetFullPath(const std::string &filename)
{
	char pFullPathName[1024];
	if (GetFullPathNameA(filename.c_str(), 1024, pFullPathName, nullptr) == 0)
		return std::string();
	
	return std::string(pFullPathName);
}

bool Filesystem::FileExists(const std::string &filename)
{
	std::ifstream f(filename.c_str(), std::ios::binary);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	return false;
}

void Filesystem::SetAssetsFolder(const std::string &folder)
{
	assetsFolder = folder;
}

std::string Filesystem::GetAssetsFolder()
{
	return assetsFolder;
}

void Filesystem::SetCoreDataFolder(const std::string &folder)
{
	coreDataFolder = folder;
}

std::string Filesystem::GetCoreDataFolder()
{
	return coreDataFolder;
}

std::string Filesystem::FindResourceByName(const std::string &name)
{
	std::string resourceInCoreData = coreDataFolder + "/" + name;
	std::string resourceInAssets = assetsFolder + "/" + name;

	if (FileExists(resourceInAssets))
		return resourceInAssets;
	if (FileExists(resourceInCoreData))
		return resourceInCoreData;
	if (FileExists(name))
		return name;

	return "";
}

std::string Filesystem::GetFileName(const std::string &name)
{
	auto i0 = name.find_last_of('\\');
	auto i1 = name.find_last_of('/');
	auto i = i0;
	if (i0 == std::string::npos)
	{
		i = i1;
	}
	else if (i1 != std::string::npos)
	{
		i = max(i0, i1);
	}

	if (i != std::string::npos)
	{
		return name.substr(i + 1);
	}
	else
	{
		return name;
	}
}

std::string Filesystem::RemoveLastSeparator(const std::string &path)
{
	if (path.back() == '/' || path.back() == '\\')
	{
		return path.substr(0, path.size() - 1);
	}
	else
	{
		return path;
	}
	
}

std::string Filesystem::GetDirectoryName(const std::string &path)
{
	std::string p = RemoveLastSeparator(path);
	unsigned int i = FindLastSeparator(p);
	if (i != -1)
	{
		return p.substr(0, i);
	}
	else
	{
		return p;
	}
}

unsigned int Filesystem::FindLastSeparator(const std::string &path)
{
	auto i0 = path.find_last_of('\\');
	auto i1 = path.find_last_of('/');
	auto i = i0;
	if (i0 == std::string::npos)
	{
		i = i1;
	}
	else if (i1 != std::string::npos)
	{
		i = max(i0, i1);
	}

	if (i == std::string::npos)
	{
		return -1;
	}
	else
	{
		return i;
	}
}