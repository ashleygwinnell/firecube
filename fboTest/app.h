class App : public Application
{
	bool Init();
	void Update(float time);
	void Render(float time);
	void HandleInput(float time);

	Font font;	
	Node root,node,node2;
	Program program;
	Buffer uvBuffer,vBuffer;
	FrameBuffer fbo;
	float ang;
};