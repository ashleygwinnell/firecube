class FireCubeApp : public FireCube::Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
	
};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();
	MyMainFrame *frame;
	FireCubeApp fireCubeApp;
};