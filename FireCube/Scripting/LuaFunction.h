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

	/**
	* Invokes the function
	*/
	template<class... Args>	
	sol::object operator ()(Args&&... args)
	{
		try 
		{
			return ref(std::forward<Args>(args)...);
		}
		catch (const std::exception &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
			return sol::make_object(ref.lua_state(), sol::nil);
		}
	}
	
private:
	sol::function ref;
};

}