class App : public Application
{
public:
	bool Init();
	void HandleInput(float time);
	void Render(float time);
	void Update(float time);
	
	ModelManager mm;
	Program program,program2;
	Font font;
	Model model;
	Model model2;
	Buffer vBuffer;
	Buffer uvBuffer;
	FrameBuffer fbo;
};