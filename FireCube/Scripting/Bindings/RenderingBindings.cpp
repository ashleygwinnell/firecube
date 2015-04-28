#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;
using namespace luabridge;

void SetParameter(Material *mat, const StringHash &paramName, LuaRef b)
{	
	if (b.isUserdata())
	{
		if (b.className() == "vec2")
		{
			mat->SetParameter(paramName, b.cast<vec2>());
		}
		else if (b.className() == "vec3")
		{
			mat->SetParameter(paramName, b.cast<vec3>());
		}
		else if (b.className() == "vec4")
		{
			mat->SetParameter(paramName, b.cast<vec4>());
		}
	}
	else if (b.isNumber())
	{
		mat->SetParameter(paramName, (float)b);
	}	
}

void LuaBindings::InitRendering(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.beginClass<Renderer>("Renderer")
			.addProperty("width", &Renderer::GetWidth)
			.addProperty("height", &Renderer::GetHeight)
		.endClass()
		.deriveClass<Material, Resource>("Material")
			.addFunctionFree("SetParameter", &SetParameter)
			.addFunction("Clone", &Material::Clone)
		.endClass();

	setGlobal(luaState, PARAM_MATERIAL_DIFFUSE, "PARAM_MATERIAL_DIFFUSE");
	setGlobal(luaState, PARAM_MATERIAL_SPECULAR, "PARAM_MATERIAL_SPECULAR");
	setGlobal(luaState, PARAM_MATERIAL_SHININESS, "PARAM_MATERIAL_SHININESS");	
}
