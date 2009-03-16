#include <string>
#include <vector>
#include <map>
#include <fstream>
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
string FireCube::GetFileName(const string &file)
{
	string::size_type i=file.find_last_of("/");
	string::size_type i2=file.find_last_of("\\");
	string::size_type maxi=0;
	
	if ((i!=string::npos) && (i2!=string::npos))
	{
		maxi=i>i2 ? i+1 : i2+1;
	}
	else if (i!=string::npos)
		maxi=i+1;
	else if (i2!=string::npos)
		maxi=i2+1;

	return file.substr(maxi);
}
bool FireCube::FileExists(const string &file)
{
	ifstream f(file.c_str(),ios::binary);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	return false;
}