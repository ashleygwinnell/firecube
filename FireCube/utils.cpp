#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

inline bool FireCube::IsFinite(float val)
{
	return ((val < 1e8) && (val > -1e8));
}
inline float FireCube::RangedRandom(float v1,float v2)
{
	return v1 + (v2-v1)*((float)rand())/((float)RAND_MAX);
}
string FireCube::ToLower(const string &str)
{
	string ret;
	string::const_iterator i=str.begin();
	for (;i!=str.end();i++)
		ret+=tolower(*i);
	return ret;
}
string FireCube::ToUpper(const string &str)
{
	string ret;
	string::const_iterator i=str.begin();
	for (;i!=str.end();i++)
		ret+=toupper(*i);
	return ret;
}
