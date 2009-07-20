#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <queue>
using namespace std;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
#include <FireCube.h>
using namespace FireCube;

int main(int argc, char *argv[])
{
	Application::InitializeLua();
	Application::ExecuteFile("script.lua");
	Application::CloseLua();
}