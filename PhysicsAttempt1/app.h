class App : public Application
{
public:
	bool HandleInput(float t);
	bool Render(float t);
	bool Update(float t);
	ModelManager modelManager;
	Model model,sphere;
	Shader vShader,pShader,pShader2;
	Program program,program2;
	Font font;
};