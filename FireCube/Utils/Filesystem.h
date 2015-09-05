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

	void FIRECUBE_API SetAssetsFolder(const std::string &folder);
	std::string FIRECUBE_API GetAssetsFolder();

	void FIRECUBE_API SetCoreDataFolder(const std::string &folder);
	std::string FIRECUBE_API GetCoreDataFolder();

	std::string FIRECUBE_API FindResourceByName(const std::string &name);

	std::string FIRECUBE_API GetFileName(const std::string &name);

	std::string FIRECUBE_API RemoveLastSeparator(const std::string &path);

	std::string FIRECUBE_API GetDirectoryName(const std::string &path);
	
	unsigned int FIRECUBE_API FindLastSeparator(const std::string &path);

	extern const char PATH_SEPARATOR_CHAR;
	extern const std::string PATH_SEPARATOR;
}
}
