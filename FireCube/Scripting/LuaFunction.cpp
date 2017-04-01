#include "Scripting/LuaFunction.h"

using namespace FireCube;

LuaFunction::LuaFunction(const sol::function &ref) : ref(ref)
{

}