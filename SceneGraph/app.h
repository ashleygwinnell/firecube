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
	Font font;	
	Program program;
	SceneGraph sg;
};