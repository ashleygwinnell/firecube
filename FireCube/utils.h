#ifndef UTIL_H
#define UTIL_H
/**
* Checks whether a given float is finite.
*/
inline bool FIRECUBE_API IsFinite(float val);
/**
* Returns a random float between v1 and v2.
*/
inline float FIRECUBE_API RangedRandom(float v1,float v2);
string FIRECUBE_API ToLower(const string &str);
string FIRECUBE_API ToUpper(const string &str);
string FIRECUBE_API GetFileName(const string &file);
bool FIRECUBE_API FileExists(const string &file);
#endif