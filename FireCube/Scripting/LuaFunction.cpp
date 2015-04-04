#include "Scripting/LuaFunction.h"

using namespace FireCube;
using namespace luabridge;

LuaFunction::LuaFunction(const luabridge::LuaRef &ref) : ref(ref)
{

}