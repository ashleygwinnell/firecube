#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"

using namespace FireCube;
using namespace luabridge;

void LuaBindings::InitRendering(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Renderer>("Renderer")
			.addProperty("width", &Renderer::GetWidth)
			.addProperty("height", &Renderer::GetHeight)
		.endClass()
		.deriveClass<Material, Resource>("Material")
		.endClass();
}
