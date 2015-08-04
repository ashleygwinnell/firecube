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
		auto bClassName = b.className();
		if (bClassName == "class<vec2>")
		{
			mat->SetParameter(paramName, b.toValue<vec2>());
		}
		else if (bClassName == "class<vec3>")
		{
			mat->SetParameter(paramName, b.toValue<vec3>());
		}
		else if (bClassName == "class<vec4>")
		{
			mat->SetParameter(paramName, b.toValue<vec4>());
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
