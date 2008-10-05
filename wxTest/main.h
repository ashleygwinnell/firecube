class FireCubeApp : public FireCube::Application
{
public:
	bool HandleInput(float t);
	bool Render(float t);
	bool Update(float t);
	void LoadModel(const string &filename);

	FireCube::Model model;
	FireCube::Font font;
	FireCube::Shader vshader,fshader;
	FireCube::Program program;
	FireCube::ModelManager mm;
	FireCube::Buffer normalRenderingBuffer;
	int normalRenderingBufferSize;
	FireCube::Program normalRenderingProgram;
};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	MyMainFrame *frame;
	FireCubeApp fireCubeApp;
};