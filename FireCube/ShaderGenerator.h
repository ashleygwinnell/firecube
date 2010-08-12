namespace FireCube
{
class FIRECUBE_API RenderState
{
	friend class ShaderGenerator;
public:
	RenderState();
	void FromMaterial(Material mat);
	void SetDirectionalLighting(bool enable);
	void SetPointLighting(bool enable);
	void SetFog(bool enable);
	unsigned int ToInt();
private:
	bool directionalLighting;
	bool pointLighting;
	bool diffuseTexture;
	bool fog;
};
class FIRECUBE_API ShaderGenerator
{
public:
	Program GenerateProgram(RenderState &renderState);
private:
	map<unsigned int,Program> programs;
};
}