#pragma once

#include <memory>
#include "Utils/utils.h"

namespace FireCube
{

class StringHash;

/**
* Specifies the type of a shader.
*/
enum ShaderType
{
	VERTEX_SHADER, FRAGMENT_SHADER
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

extern StringHash BASE_PASS;
extern StringHash LIGHT_PASS;

}