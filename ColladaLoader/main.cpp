#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
using namespace std;
#include <FireCube.h>
using namespace FireCube;
#include "tinyxml.h"
#include "ColladaLoader.h"

class App : public Application
{
public:
	virtual void Update(float t);
	virtual void HandleInput(float t);
	virtual void Render(float t);
	Model m;
};
void App::Update(float t)
{

}
void App::HandleInput(float t)
{

}
void App::Render(float t)
{
	Renderer::Clear(vec4(0,0,0,1),1);	
	Renderer::Render(m);
}
void main()
{
	App app;
	ColladaLoader l("../Media/Models/duck_triangulate.dae");
	l.Load();	
	app.Initialize();
	app.m=l.TempGenModel();
	app.m->UpdateBuffers();
	mat4 m;
	m.Translate(0,0,-20);
	Renderer::SetModelViewMatrix(m);
	Renderer::SetPerspectiveProjection(60.0f,0.1f,2000.0f);

	app.Run();
	cout << "All OK." << endl;
}