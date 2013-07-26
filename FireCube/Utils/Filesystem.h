#pragma once

#include <vector>
#include <string>

#include "Utils/utils.h"

namespace FireCube
{

/**
* A namespace containing various functions to interact with the file system.
*/
namespace Filesystem
{
	/**
	* Add a search path for resources.
	* @param path The path to add.
	*/
	void FIRECUBE_API AddSearchPath(const std::string &path);

	/**
	* Gets the list of search paths.
	* @return The list of search paths.
	*/
	const FIRECUBE_API std::vector<std::string> &GetSearchPaths();

	/**
	* Searches for file name using the search paths.
	* @param filename The file name to search.
	* @return A path to the file if it was found or an empty string if it wasn't.
	*/
	std::string FIRECUBE_API SearchForFileName(const std::string &filename);

	/**
	* Returns the full path of the given file.	
	* @param filename The file name (can be relative or absolute).
	* @return The absolute path of the given file name.
	*/
	std::string FIRECUBE_API GetFullPath(const std::string &filename);

	/**
	* Checks whether the given file exists.
	* @param filename the file to check.
	* @return True if the file exists, false otherwise.
	*/
	bool FIRECUBE_API FileExists(const std::string &filename);
}
}
