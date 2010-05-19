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
#include "ShaderGenerator.h"
#include "Light.h"
#include "SceneGraph.h"
#include "app.h"
#include <cmath>
App app;
int main(int argc, char *argv[])
{	
	if (!app.Initialize(1024,768,32,0,false))
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
	sg.AddModel("duck","duck.dae");
	sg.AddModel("teapot","teapot.3ds");	
	sg.AddModel("collada","collada.dae");
	sg.GetModel("teapot")->CreateHardNormals();
	Light l=sg.AddLight("Light1");
	l->ambientColor.Set(1,1,1,1);
	l->diffuseColor.Set(0.7f,0.7f,0.7f,1);
	l->type=DIRECTIONAL;

	Node n=sg.Root()->AddChild("Node1");
	n->Move(vec3(0,-2,-10));	
	n->AttachModel("collada");
	
	n=sg.Root()->AddChild("Node2");
	n->Move(vec3(8,-2,-10));	
	n->AttachModel("teapot");
	
	n=sg.Root()->AddChild("Node3");	
	n->Scale(vec3(0.03f,0.03f,0.03f));
	n->Move(vec3(-8,-4,-10));
	n->AttachModel("duck");		
	
	n=sg.Root()->AddChild("LightNode1");	
	n->AttachLight("Light1");
	sg.SetFog(true);
	sg.SetFogColor(vec4(0.2f,0.2f,0.6f,1.0f));
	sg.SetFogDensity(0.05f);
	return true;
}
void App::Update(float t)
{
	static float time=0.0f;
	time+=t;
	sg.GetNode("Node3")->Rotate(vec3(0,(float)(PI/4*t),0));		
	sg.GetNode("Node3")->Move(2.0f*sin(time)*vec3(0,0,-0.1f));
	sg.GetNode("Node2")->Rotate(vec3(0,(float)(PI/4*t),0));
	sg.GetNode("Node1")->Rotate(vec3((float)(PI/8*t),(float)(PI/8*t),0));	
}
void App::Render(float t)
{
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,1000.0f);	
	Renderer::SetModelViewMatrix(mat4());
	sg.Render();
	Renderer::SetModelViewMatrix(mat4());
	Renderer::SetOrthographicProjection();	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec2(0,(float)app.GetHeight()-20.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{	
}
