class App : public Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);	
	Node model,sphere;
	Shader vShader,pShader,pShader2;
	Program program,program2;
	Font font;
};