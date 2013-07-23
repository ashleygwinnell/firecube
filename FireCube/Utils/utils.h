#ifndef UTIL_H
#define UTIL_H

#ifdef FIRECUBE_DYNAMIC_LIBRARY
	#ifdef FIRECUBE_EXPORTS
		#define FIRECUBE_API __declspec(dllexport)
	#else
		#define FIRECUBE_API __declspec(dllimport)		
	#endif
#else
	#ifdef FIRECUBE_EXPORTS
		#define FIRECUBE_API
	#else
		#define FIRECUBE_API
	#endif
#endif

/**
* Main namespace of the engine.
*/
namespace FireCube
{
/**
* Checks whether a given float is finite.
*/
bool FIRECUBE_API IsFinite(float val);
/**
* Returns a random float between v1 and v2.
*/
float FIRECUBE_API RangedRandom(float v1, float v2);

/**
* Converts a string into lower case.
*/
std::string FIRECUBE_API ToLower(const std::string &str);

/**
* Converts a string into upper case.
*/
std::string FIRECUBE_API ToUpper(const std::string &str);

/**
* Gets the file name of a path.
*/
std::string FIRECUBE_API GetFileName(const std::string &file);

/**
* Specifies the maximum number of textures in various locations.
*/
const unsigned int MAX_TEXTURES = 6;
};
#endif