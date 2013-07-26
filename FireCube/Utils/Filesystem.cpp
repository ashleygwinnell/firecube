#include <fstream>

#include <Windows.h>
#include "Utils/Filesystem.h"

using namespace FireCube;

std::vector<std::string> searchPaths;

void Filesystem::AddSearchPath(const std::string &path)
{
	std::string npath = path;
	if ((npath[npath.size() - 1] == '\\') || (npath[npath.size() - 1] == '/'))
		npath = npath.substr(0, npath.size() - 1);
	searchPaths.push_back(npath);
}

const std::vector<std::string> &Filesystem::GetSearchPaths()
{
	return searchPaths;
}

std::string Filesystem::SearchForFileName(const std::string &filename)
{	
	std::string ret;
	if (!FileExists(filename))
	{
		const std::vector<std::string> &searchPaths = Filesystem::GetSearchPaths();
		for (unsigned int i = 0; i < searchPaths.size(); i++)
		{
			ret = searchPaths[i] + "\\" + filename;
			if (FileExists(ret))
				return ret;
		}
	}
	else
		return filename;

	return "";
}

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