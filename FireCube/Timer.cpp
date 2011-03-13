#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"
#include "utils.h"
#include "Timer.h"

using namespace FireCube;

void Timer::Init()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&now);
    res = (double)(1.0 / (double)freq.QuadPart);
}
void Timer::Update()
{
    QueryPerformanceCounter(&now);
}
double Timer::Passed()
{
    QueryPerformanceCounter(&temp);
    return (temp.QuadPart - now.QuadPart) * res;
}
