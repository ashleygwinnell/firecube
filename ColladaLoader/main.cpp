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
	static float ang=0;
	ang=ang+t;
	Renderer::Clear(vec4(0.5f,0.5f,0.5f,1),1);	
	mat4 ma;
	ma.Translate(0,-70,-180);
	//ma.Translate(0,-0,-10);
	ma.RotateY(ang);
	Renderer::SetModelViewMatrix(ma);
	Renderer::Render(m);
}
void main()
{
	Application::AddSearchPath("../Media/Textures");
	App app;
	ColladaLoader l("../Media/Models/duck_triangulate.dae");
	l.Load();	
	app.Initialize();
	app.m=Model(new ModelResource());
	l.GenerateModel(app.m.get());
	Program p(new ProgramResource());
	p->Create(Renderer::GetShaderManager().Create("plainTexture.vshader"),Renderer::GetShaderManager().Create("plainTexture.fshader"));
	//p->Create(Renderer::GetShaderManager().Create("plainColor.vshader"),Renderer::GetShaderManager().Create("plainColor.fshader"));
	cout << p->GetInfoLog();	
	app.m->UpdateBuffers();
	app.m->SetProgram(p);
	
	Renderer::SetPerspectiveProjection(60.0f,0.1f,2000.0f);	
	app.Run();
	cout << "All OK." << endl;
}