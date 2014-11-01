#include "Project.h"

class FireCubeApp : public FireCube::Application
{
public:
	void Render(float t) {};
	void Update(float t) {};

};

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	Project &GetProject();
	FireCubeApp fcApp;
private:	
	MainFrameImpl *frame;
	Project project;
};