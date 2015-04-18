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
		try
		{
			ref(std::forward<Args>(args)...);
		}
		catch (luabridge::LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}		
	}
	
private:
	luabridge::LuaRef ref;
};

}