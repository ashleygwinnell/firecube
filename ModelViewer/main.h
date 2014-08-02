class FireCubeApp : public FireCube::Application
{
public:    
    void Render(float t);
    void Update(float t);
    
};
class ApplicationParameters
{
public:
	bool customProgram;
	float normalsLength;
	FireCube::Shader *vshader, *fshader;
	FireCube::Program *program;
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
	void LoadDocument(const std::string &filename);
	Document &GetDocument();
	ApplicationParameters &GetApplicationParameters();	
	FireCubeApp fcApp;
private:
    MyMainFrame *frame;
	ApplicationParameters applicationParameters;
	Document document;
	
};