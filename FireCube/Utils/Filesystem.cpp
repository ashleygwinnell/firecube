#include "Utils/Logger.h"
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include "Utils/Filesystem.h"

using namespace FireCube;

std::string assetsFolder = "";
std::string coreDataFolder = "";
const char Filesystem::PATH_SEPARATOR_CHAR = '\\';
const std::string Filesystem::PATH_SEPARATOR = "\\";

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
	std::string resourceInCoreData = coreDataFolder + PATH_SEPARATOR + name;
	std::string resourceInAssets = assetsFolder + PATH_SEPARATOR + name;

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
	auto i = name.find_last_of(PATH_SEPARATOR_CHAR);

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
	if (path.back() == PATH_SEPARATOR_CHAR)
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
	auto i = path.find_last_of(PATH_SEPARATOR_CHAR);

	if (i == std::string::npos)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

bool Filesystem::IsSubPathOf(const std::string &base, const std::string &path)
{
	std::string baseFull = GetFullPath(base);
	std::string pathFull = GetFullPath(path);

	return pathFull.size() >= baseFull.size() && _strnicmp(baseFull.c_str(), pathFull.c_str(), baseFull.size()) == 0;
}

std::string Filesystem::MakeRelativeTo(const std::string &base, const std::string &path)
{
	if (!IsSubPathOf(base, path))
		return "";

	std::string baseFull = GetFullPath(base);
	std::string pathFull = GetFullPath(path);
	std::string pathRelative = pathFull.substr(baseFull.size());
	return pathRelative;
}

std::string Filesystem::GetLastPathComponent(const std::string &path)
{
	std::string p = RemoveLastSeparator(path);
	unsigned int i = FindLastSeparator(p);
	if (i != -1)
	{
		return p.substr(i + 1);
	}
	else
	{
		return p;
	}
}

bool Filesystem::CopyPath(const std::string &source, const std::string &destination)
{
	return CopyFileA(source.c_str(), destination.c_str(), 0) == TRUE;	
}

bool Filesystem::CreateFolder(const std::string &path)
{
	BOOL b = CreateDirectoryA(path.c_str(), nullptr);

	if (b != TRUE && GetLastError() != ERROR_ALREADY_EXISTS)
	{
		return false;		
	}
	else
	{
		return true;
	}
}
