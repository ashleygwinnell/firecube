#include "Rendering/RenderingTypes.h"
#include "Geometry/Material.h"
#include "Utils/StringHash.h"

namespace FireCube
{

StringHash PARAM_MATERIAL_AMBIENT("materialAmbient");
StringHash PARAM_MATERIAL_DIFFUSE("materialDiffuse");
StringHash PARAM_MATERIAL_SPECULAR("materialSpecular");
StringHash PARAM_MATERIAL_SHININESS("materialShininess");
StringHash PARAM_AMBIENT_COLOR("ambientColor");
StringHash PARAM_MODEL_MATRIX("modelMatrix");
StringHash PARAM_VIEW_PROJECTION_MATRIX("viewProjectionMatrix");
StringHash PARAM_NORMAL_MATRIX("normalMatrix");
StringHash PARAM_LIGHT_DIR("directionalLightDir");
StringHash PARAM_LIGHT_POS("lightPosition");
StringHash PARAM_LIGHT_DIFFUSE("lightDiffuse");
StringHash PARAM_LIGHT_SPECULAR("lightSpecular");
StringHash PARAM_LIGHT_SPOT_DIR("spotLightDir");
StringHash PARAM_CAMERA_POS("cameraPos");

StringHash BASE_PASS("base");
StringHash LIGHT_PASS("light");


}