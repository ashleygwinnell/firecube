#include "Rendering/RenderingTypes.h"
#include "Utils/StringHash.h"

namespace FireCube
{

StringHash PARAM_MATERIAL_DIFFUSE("materialDiffuse");
StringHash PARAM_MATERIAL_SPECULAR("materialSpecular");
StringHash PARAM_MATERIAL_SHININESS("materialShininess");
StringHash PARAM_MATERIAL_OPACITY("materialOpacity");
StringHash PARAM_U_OFFSET("uOffset");
StringHash PARAM_V_OFFSET("vOffset");
StringHash PARAM_AMBIENT_COLOR("ambientColor");
StringHash PARAM_MODEL_MATRIX("modelMatrix");
StringHash PARAM_VIEW_PROJECTION_MATRIX("viewProjectionMatrix");
StringHash PARAM_PROJECTION_MATRIX("projectionMatrix");
StringHash PARAM_NORMAL_MATRIX("normalMatrix");
StringHash PARAM_LIGHT_DIR("directionalLightDir");
StringHash PARAM_LIGHT_POS("lightPosition");
StringHash PARAM_LIGHT_COLOR("lightColor");
StringHash PARAM_LIGHT_SPOT_DIR("spotLightDir");
StringHash PARAM_CAMERA_POS("cameraPos");
StringHash PARAM_FOG_PARAMETERS("fogParameters");
StringHash PARAM_FOG_COLOR("fogColor");
StringHash PARAM_TIME_STEP("timeStep");
StringHash PARAM_SKIN_MATRICES("skinMatrices");
StringHash PARAM_LIGHT_BIAS_MVP_MATRIX("lightBiasMVPMatrix");
StringHash PARAM_SHADOW_INTENSITY("shadowIntensity");
StringHash PARAM_VIEWPORT_INV_SIZE("viewportInvSize");
StringHash PARAM_SCREEN_SIZE("screenSize");

StringHash BASE_PASS("base_opaque");
StringHash LIGHT_PASS("light_opaque");
StringHash SHADOW_PASS("shadow");
StringHash ALPHA_PASS("base_alpha");

StringHash VIEWPORT_TARGET("viewport");

ClearBufferType FireCube::operator | (const ClearBufferType &lhs, const ClearBufferType &rhs)
{
	ClearBufferType ret = static_cast<ClearBufferType>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return ret;
}

ClearBufferType FireCube::operator |= (ClearBufferType &lhs, const ClearBufferType &rhs)
{
	lhs = static_cast<ClearBufferType>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return lhs;
}

ClearBufferType FireCube::operator & (const ClearBufferType &lhs, const ClearBufferType &rhs)
{
	ClearBufferType ret = static_cast<ClearBufferType>(static_cast<int>(lhs)& static_cast<int>(rhs));
	return ret;
}

}