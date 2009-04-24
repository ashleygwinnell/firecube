#include <string>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

ofstream file;
void Logger::Init(string filename)
{
	file.open(filename.c_str(),ios::out|ios::trunc);	
}
void Logger::Write(string str)
{
	if (file.is_open())
		file << str;
}
void Logger::Close()
{
	file.close();
}
