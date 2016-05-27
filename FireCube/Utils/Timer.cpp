#include "Utils/Timer.h"

using namespace FireCube;



Timer::Timer()
{
	Init();
}

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

float Timer::Passed() const
{
	LARGE_INTEGER temp;
    QueryPerformanceCounter(&temp);
    return (float) ((temp.QuadPart - now.QuadPart) * res);
}
