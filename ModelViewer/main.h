class FireCubeApp : public FireCube::Application
{
public:
    void HandleInput(float t);
    void Render(float t);
    void Update(float t);
    
};
class ApplicationParameters
{
public:
	bool customProgram;
	float normalsLength;
	FireCube::ShaderPtr vshader, fshader;
	FireCube::ProgramPtr program;
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
	void LoadDocument(const string &filename);
	Document &GetDocument();
	ApplicationParameters &GetApplicationParameters();	
	FireCubeApp fcApp;
private:
    MyMainFrame *frame;
	ApplicationParameters applicationParameters;
	Document document;
	
};