#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"
#include "Rendering/RenderingTypes.h"

using namespace FireCube;

void LuaBindings::InitRendering(sol::state &luaState)
{
	luaState.new_usertype<Renderer>("Renderer",
		"width", sol::property(&Renderer::GetWidth),
		"height", sol::property(&Renderer::GetHeight));
	luaState.new_usertype<Material>("Material",
		"SetParameter", sol::overload(
			sol::resolve<void(const std::string &, float)>(&Material::SetParameter),
			sol::resolve<void(const std::string &, int)>(&Material::SetParameter),
			sol::resolve<void(const std::string &, const vec2 &)>(&Material::SetParameter),
			sol::resolve<void(const std::string &, const vec3 &)>(&Material::SetParameter),
			sol::resolve<void(const std::string &, const vec4 &)>(&Material::SetParameter)),
		"GetParameterVec3", [](Material &self, const std::string &paramName) {
			return self.GetParameterValue(paramName).GetVec3();
		},
		"Clone", &Material::Clone,
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	luaState["PARAM_MATERIAL_DIFFUSE"] = PARAM_MATERIAL_DIFFUSE;
	luaState["PARAM_MATERIAL_SPECULAR"] = PARAM_MATERIAL_SPECULAR;
	luaState["PARAM_MATERIAL_SHININESS"] = PARAM_MATERIAL_SHININESS;
	luaState["PARAM_MATERIAL_OPACITY"] = PARAM_MATERIAL_OPACITY;	
}
