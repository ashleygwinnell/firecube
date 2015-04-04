#include "Core/EventSystem.h"

namespace FireCube
{

class MappedInput;

namespace Events
{

extern Event<float> Update;
extern Event<float, const MappedInput &> HandleInput;

}

}