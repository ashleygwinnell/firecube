#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;
#include <windows.h>
#include "Utils/utils.h"
#include "Utils/Timer.h"

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