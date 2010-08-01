namespace FireCube
{
enum LightType
{
	DIRECTIONAL,POINT
};
class FIRECUBE_API LightResource
{
public:
	LightType type;
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;	
};
typedef boost::shared_ptr<LightResource> Light;
}