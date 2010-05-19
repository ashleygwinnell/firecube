class RenderState
{
	friend class ShaderGenerator;
public:
	RenderState();
	void FromMaterial(Material mat);
	void SetDirectionalLighting(bool enable);
	void SetFog(bool enable);
	DWORD ToInt();
private:
	bool directionalLighting;
	bool diffuseTexture;
	bool fog;
};
class ShaderGenerator
{
public:
	Program GenerateProgram(RenderState &renderState);
private:
	map<DWORD,Program> programs;
};