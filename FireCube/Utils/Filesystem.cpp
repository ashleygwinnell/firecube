#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <fstream>
using namespace std;

#include <Windows.h>
#include "Utils/utils.h"
#include "Utils/Filesystem.h"

using namespace FireCube;

vector<string> searchPaths;

void Filesystem::AddSearchPath(const string &path)
{
	string npath = path;
	if ((npath[npath.size() - 1] == '\\') || (npath[npath.size() - 1] == '/'))
		npath = npath.substr(0, npath.size() - 1);
	searchPaths.push_back(npath);
}

const vector<string> &Filesystem::GetSearchPaths()
{
	return searchPaths;
}

string Filesystem::SearchForFileName(const string &filename)
{	
	string ret;
	if (!FileExists(filename))
	{
		const vector<string> &searchPaths = Filesystem::GetSearchPaths();
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

string Filesystem::GetFullPath(const std::string &filename)
{
	char pFullPathName[1024];
	if (GetFullPathNameA(filename.c_str(), 1024, pFullPathName, nullptr) == 0)
		return string();
	
	return string(pFullPathName);
}

bool Filesystem::FileExists(const string &filename)
{
	ifstream f(filename.c_str(), ios::binary);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	return false;
}