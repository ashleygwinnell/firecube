#ifndef FILESYSTEM_H
#define FILESYSTEM_H

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
    */
    std::string FIRECUBE_API SearchForFileName(const std::string &filename);
}
}
#endif