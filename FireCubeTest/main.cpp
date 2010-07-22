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
App app;
#include <fstream>
#include <cmath>
vec3 lastPos;
vec3 rot(0,0,-3);
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
	SetTitle(string("FireCube Test Application"));		
	font=Renderer::GetFontManager().Create("c:\\windows\\fonts\\arial.ttf",18);
	return true;
}
void App::Update(float t)
{	
}
void App::Render(float t)
{
	Renderer::Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100.0f);	
	Renderer::SetModelViewMatrix(mat4());	
	Renderer::SetOrthographicProjection();	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	Renderer::RenderText(app.font,vec2(0,(float)app.GetHeight()-20.0f),vec4(1,1,1,1),oss.str());
}
void App::HandleInput(float t)
{
	
}
