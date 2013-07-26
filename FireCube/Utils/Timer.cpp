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
