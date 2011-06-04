#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"

#include "utils.h"
#include "Filesystem.h"

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
	bool found = false;
	string ret;
	if (!FileExists(filename))
	{
		const vector<string> &searchPaths = Filesystem::GetSearchPaths();
		for (unsigned int i = 0; i < searchPaths.size(); i++)
		{
			ret = searchPaths[i] + "\\" + filename;
			if (FileExists(ret))
			{
				found = true;
				break;
			}
		}
	}
	else
		return filename;

	if (found)
		return ret;
	else
		return "";
}