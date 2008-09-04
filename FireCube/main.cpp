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
bool APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved)
{	
	return true;
}
