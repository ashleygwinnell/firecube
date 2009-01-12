class App : public Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
	void SetupLight();
	void RenderShadowMap();
	void RenderDepth();
	bool Init();
	FrameBuffer fb;
	ModelManager modelManager;
	Model model;	
	Font font;	
	Program plain,shadowMap,program;
	mat4 lightProj,lightModelview;
};