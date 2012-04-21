#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
using namespace std;

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