#pragma once

#include "Utils/utils.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/LuaBridge/LuaBridge.h"

namespace FireCube
{

class FIRECUBE_API LuaFunction
{
public:
	LuaFunction(const luabridge::LuaRef &ref);
	template<class... Args>
	void operator ()(Args&&... args)
	{
		ref(std::forward<Args>(args)...);
	}
	
private:
	luabridge::LuaRef ref;
};

}