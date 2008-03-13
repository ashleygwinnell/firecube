#include <string>
#include <vector>
using namespace std;
#include <SDL/SDL.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

inline bool FireCube::is_finite(float val)
{
	return ((val < 1e8) && (val > -1e8));
}
