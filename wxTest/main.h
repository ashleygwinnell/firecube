class FireCubeApp : public FireCube::Application
{
public:
	bool HandleInput(float t);
	bool Render(float t);
	bool Update(float t);	
};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	MyMainFrame *frame;
	FireCubeApp fireCubeApp;
};