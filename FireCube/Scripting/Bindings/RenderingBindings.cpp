#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"
#include "Rendering/Technique.h"
#include "Rendering/RenderingTypes.h"
#include "Rendering/Font.h"
#include "Rendering/Texture2D.h"

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
		"technique", sol::property(&Material::GetTechnique, &Material::SetTechnique),
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());
	luaState.new_usertype<Technique>("Technique",
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	luaState["PARAM_MATERIAL_DIFFUSE"] = PARAM_MATERIAL_DIFFUSE;
	luaState["PARAM_MATERIAL_SPECULAR"] = PARAM_MATERIAL_SPECULAR;
	luaState["PARAM_MATERIAL_SHININESS"] = PARAM_MATERIAL_SHININESS;
	luaState["PARAM_MATERIAL_OPACITY"] = PARAM_MATERIAL_OPACITY;

	luaState.new_usertype<Font>("Font",
		"GenerateFontFace", &Font::GenerateFontFace,
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	luaState.new_usertype<FontFace>("FontFace");

	luaState.new_usertype<Texture>("Texture",
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());

	luaState.new_usertype<Texture2D>("Texture2D",
		sol::base_classes, sol::bases<Texture, Resource, Object, RefCounted>());

}
