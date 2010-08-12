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
class FIRECUBE_API Light
{
public:
	void Create();
	void SetType(LightType type);
	LightType GetType();
	void SetAmbientColor(vec4 color);
	vec4 GetAmbientColor();
	void SetDiffuseColor(vec4 color);
	vec4 GetDiffuseColor();
	void SetSpecularColor(vec4 color);
	vec4 GetSpecularColor();

	operator bool () const;
	bool operator== (const Light &light) const;
private:
	boost::shared_ptr<LightResource> resource;
};
}