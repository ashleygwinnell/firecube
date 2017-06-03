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
	extern const char PATH_SEPARATOR_CHAR;
	extern const std::string PATH_SEPARATOR;

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

	bool FIRECUBE_API IsSubPathOf(const std::string &base, const std::string &path);

	std::string FIRECUBE_API MakeRelativeTo(const std::string &base, const std::string &path);

	std::string FIRECUBE_API GetLastPathComponent(const std::string &path);

	bool FIRECUBE_API CopyPath(const std::string &source, const std::string &destination);	

	bool FIRECUBE_API CreateFolder(const std::string &path);

	std::string FIRECUBE_API GetFileExtension(const std::string &filename);

	std::string FIRECUBE_API RemoveFileExtension(const std::string &filename);

	std::string FIRECUBE_API AddPathSeparatorIfNeeded(const std::string &path);

	template <typename ...Args>
	std::string JoinPath(const std::string &p0, const std::string &p1, Args const&... args)
	{
		return p0 + Filesystem::PATH_SEPARATOR + JoinPath(p1, args...);
	}
	
	inline std::string JoinPath(const std::string &p0)
	{
		return p0;
	}
}
}
