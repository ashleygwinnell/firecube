#pragma once

#include "Core/Memory.h"
#include "Utils/utils.h"
#include "ThirdParty/Lua/src/lua.hpp"
#include "ThirdParty/LuaIntf/LuaIntf.h"

namespace FireCube
{

/**
* This class represnets a referne to a Lua function
*/
class FIRECUBE_API LuaFunction : public RefCounted
{
public:
	LuaFunction(const LuaIntf::LuaRef &ref);
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
		catch (LuaIntf::LuaException &e)
		{
			(void)e; // Disable warning about e not being used
			LOGERROR(e.what());
		}		
	}
	
private:
	LuaIntf::LuaRef ref;
};

}