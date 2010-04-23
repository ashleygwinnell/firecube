class App : public Application
{
	bool Init();
	void Update(float time);
	void Render(float time);
	void HandleInput(float time);

	Font font;
	ModelManager mm;
	Model model,model2;
	Program program,program2;
	Buffer uvBuffer,vBuffer;
	FrameBuffer fbo;
	float ang;
};