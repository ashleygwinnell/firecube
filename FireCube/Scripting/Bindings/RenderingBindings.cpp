#include "lua.hpp"
#include "LuaIntf.h"
#include "Scripting/LuaBindings.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;
using namespace LuaIntf;

void SetParameter(Material *mat, lua_State *L, const StringHash &paramName, LuaRef b)
{	
	if (b.type() == LuaTypeID::USERDATA)
	{
		vec2 *v2;
		vec3 *v3;
		vec4 *v4;
		
		if (v2 = Lua::objectCast<vec2>(b))
		{
			mat->SetParameter(paramName, *v2);
		}
		else if (v3 = Lua::objectCast<vec3>(b))
		{
			mat->SetParameter(paramName, *v3);
		}
		else if (v4 = Lua::objectCast<vec4>(b))
		{
			mat->SetParameter(paramName, *v4);
		}
	}
	else if (b.type() == LuaTypeID::NUMBER)
	{
		mat->SetParameter(paramName, b.toValue<float>());
	}	
}

void LuaBindings::InitRendering(lua_State *luaState)
{
	LuaBinding(luaState)
		.beginClass<Renderer>("Renderer")
			.addProperty("width", &Renderer::GetWidth)
			.addProperty("height", &Renderer::GetHeight)
		.endClass()
		.beginExtendClass<Material, Resource>("Material")
			.addFunction("SetParameter", &SetParameter)
			.addFunction("Clone", &Material::Clone)
		.endClass();

	Lua::setGlobal(luaState, "PARAM_MATERIAL_DIFFUSE", PARAM_MATERIAL_DIFFUSE);
	Lua::setGlobal(luaState, "PARAM_MATERIAL_SPECULAR", PARAM_MATERIAL_SPECULAR);
	Lua::setGlobal(luaState, "PARAM_MATERIAL_SHININESS", PARAM_MATERIAL_SHININESS);
}
