enum LightType
{
	DIRECTIONAL
};
class LightResource
{
public:
	LightType type;
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	float specularExponent;
};
typedef boost::shared_ptr<LightResource> Light;