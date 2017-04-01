#pragma once

#include "Core/Memory.h"
#include "Utils/utils.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/Sol/sol.hpp"

namespace FireCube
{

/**
* This class represents a reference to a Lua function
*/
class FIRECUBE_API LuaFunction : public RefCounted
{
public:
	LuaFunction(const sol::function &ref);
	template<class... Args>
	
	/**
	* Invokes the function
	*/
	void operator ()(Args&&... args)
	{
		try 
		{
			ref(std::forward<Args>(args)...);
		}
		catch (const std::exception &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}
	}
	
private:
	sol::function ref;
};

}