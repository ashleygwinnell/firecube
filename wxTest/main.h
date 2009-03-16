class FireCubeApp : public FireCube::Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
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