#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <queue>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
#include "SceneGraph.h"
App app;
SceneGraph sg;
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
		return 0;		
	app.Run();			
	return 0;
}
bool App::Init()
{
	Application::AddSearchPath("../Media/Textures");
	Application::AddSearchPath("../Media/Models");
	SetTitle("SceneGraph Test Application");		
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",18);
	program=Program(new ProgramResource);
	program->Create(Renderer::GetShaderManager().Create("plainTexture.vshader"),Renderer::GetShaderManager().Create("plainTexture.fshader"));
	mat4 mtr;
	mtr.Translate(0,-3,-10);
	mtr.Scale(0.05f,0.05f,0.05f);
	Node n=sg.Root()->AddChild("Node1",mtr);	
	Model m=sg.AddModel("duck","duck.dae");
	m->SetProgram(program);
	n->AttachModel(m);
	return true;
}
void App::Update(float t)
{
	sg.GetNode("Node1")->GetTransformation().RotateY((float)(PI/2*t));	
}
void App::Render(float t)
{
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100.0f);	
	Renderer::SetModelViewMatrix(mat4());
	sg.Render();
	Renderer::SetOrthographicProjection();	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec2(0,(float)app.GetHeight()-20.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{	
}
