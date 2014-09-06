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
	MAX_TEXTURE_UNITS
};

enum ShaderPermutation
{
	SP_SPOT_LIGHT,
	SP_POINT_LIGHT,
	SP_DIRECTIONAL_LIGHT,
	MAX_SHADER_PERMUTATIONS
};

enum class RenderSurfaceType
{
	COLOR,
	DEPTH
};

enum class BlendMode
{
	REPLACE, ADD
};

enum class DepthTest
{
	ALWAYS, NEVER, EQUAL, LESSEQUAL, LESS
};

extern StringHash PARAM_MATERIAL_AMBIENT;
extern StringHash PARAM_MATERIAL_DIFFUSE;
extern StringHash PARAM_MATERIAL_SPECULAR;
extern StringHash PARAM_MATERIAL_SHININESS;
extern StringHash PARAM_AMBIENT_COLOR;
extern StringHash PARAM_MODEL_MATRIX;
extern StringHash PARAM_VIEW_PROJECTION_MATRIX;
extern StringHash PARAM_NORMAL_MATRIX;
extern StringHash PARAM_LIGHT_DIR;
extern StringHash PARAM_LIGHT_POS;
extern StringHash PARAM_LIGHT_DIFFUSE;
extern StringHash PARAM_LIGHT_SPECULAR;
extern StringHash PARAM_LIGHT_SPOT_DIR;
extern StringHash PARAM_CAMERA_POS;
extern StringHash PARAM_FOG_PARAMETERS;
extern StringHash PARAM_FOG_COLOR;
extern StringHash PARAM_LIGHT_MATRIX;
extern StringHash PARAM_TIME_STEP;
extern StringHash PARAM_LIFE_TIME;

extern StringHash BASE_PASS;
extern StringHash LIGHT_PASS;

}