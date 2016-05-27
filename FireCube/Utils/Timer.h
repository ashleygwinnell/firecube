#pragma once

#include <windows.h>
#include "Utils/utils.h"

namespace FireCube
{
/**
* A class for a timer.
*/
class FIRECUBE_API Timer
{
public:

	Timer();

    /**
    * Initializes the timer.
    */
    void Init();

    /**
    * Resets the timer.
    */
    void Update();

    /**
    * Returns the time that has passed since the last call to Update() in seconds.
    */
    float Passed() const;
private:
    LARGE_INTEGER now;
    
    double res;
};
}
