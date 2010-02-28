#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "tinyxml.h"
#include "ColladaLoader.h"
void main()
{
	ColladaLoader l("../Media/Models/duck_triangulate.dae");
	l.Load();
	cout << "All OK." << endl;
}