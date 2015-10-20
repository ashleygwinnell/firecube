#pragma once

#include <memory>
#include "Utils/utils.h"

namespace FireCube
{

class StringHash;

/**
* Specifies the type of a shader.
*/
enum class ShaderType
{
	VERTEX_SHADER, FRAGMENT_SHADER
};

enum class TextureUnit
{
	DIFFUSE,
	NORMAL,
	SPECULAR,
	MAX_TEXTURE_UNITS,
	SHADOWMAP = 3,
};

enum VertexShaderLightPermutation
{
	VSP_SPOT_LIGHT,
	VSP_POINT_LIGHT,
	VSP_DIRECTIONAL_LIGHT,
	VSP_SPOT_LIGHT_SHADOW,
	VSP_POINT_LIGHT_SHADOW,
	VSP_DIRECTIONAL_LIGHT_SHADOW,
	MAX_VERTEX_SHADER_LIGHT_PERMUTATIONS
};

enum FragmentShaderLightPermutation
{
	FSP_SPOT_LIGHT,
	FSP_POINT_LIGHT,
	FSP_DIRECTIONAL_LIGHT,
	FSP_SPOT_LIGHT_SHADOW,
	FSP_POINT_LIGHT_SHADOW,
	FSP_DIRECTIONAL_LIGHT_SHADOW,
	MAX_FRAGMENT_SHADER_LIGHT_PERMUTATIONS
};


enum class RenderSurfaceType
{
	COLOR,
	DEPTH,
	DEPTH_TEXTURE
};

enum class BlendMode
{
	REPLACE, ADD, ALPHA, ADDALPHA
};

enum class DepthTest
{
	ALWAYS, NEVER, EQUAL, LESSEQUAL, LESS
};

enum class ClearBufferType
{
	NONE = 0, COLOR = 1, DEPTH = 2
};

/**
* Specifies the kind of primitives to render.
*/
enum class PrimitiveType
{
	POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, QUADS, LINE_LOOP, LINE_STRIP, TRIANGLE_FAN
};

ClearBufferType operator | (const ClearBufferType &lhs, const ClearBufferType &rhs);
ClearBufferType operator |= (ClearBufferType &lhs, const ClearBufferType &rhs);
ClearBufferType operator & (const ClearBufferType &lhs, const ClearBufferType &rhs);

extern StringHash PARAM_MATERIAL_DIFFUSE;
extern StringHash PARAM_MATERIAL_SPECULAR;
extern StringHash PARAM_MATERIAL_SHININESS;
extern StringHash PARAM_MATERIAL_OPACITY;
extern StringHash PARAM_AMBIENT_COLOR;
extern StringHash PARAM_MODEL_MATRIX;
extern StringHash PARAM_VIEW_PROJECTION_MATRIX;
extern StringHash PARAM_NORMAL_MATRIX;
extern StringHash PARAM_LIGHT_DIR;
extern StringHash PARAM_LIGHT_POS;
extern StringHash PARAM_LIGHT_COLOR;
extern StringHash PARAM_LIGHT_SPOT_DIR;
extern StringHash PARAM_CAMERA_POS;
extern StringHash PARAM_FOG_PARAMETERS;
extern StringHash PARAM_FOG_COLOR;
extern StringHash PARAM_TIME_STEP;
extern StringHash PARAM_SKIN_MATRICES;
extern StringHash PARAM_LIGHT_BIAS_MVP_MATRIX;
extern StringHash PARAM_SHAODW_INTENSITY;

extern StringHash BASE_PASS;
extern StringHash LIGHT_PASS;
extern StringHash SHADOW_PASS;
extern StringHash ALPHA_PASS;

}