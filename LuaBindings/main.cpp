#include <Windows.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void InitializeLua(lua_State *& luaState);
bool APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved)
{	
	return true;
}

extern "C"
{
	int __declspec(dllexport) luaopen_LuaFireCube(lua_State *L)
	{
		InitializeLua(L);
		return 0;
	}
};