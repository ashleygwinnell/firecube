#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
Model model;
Renderer renderer;
vec3 lastPos;
vec3 rot(0,0,-3);
Font font;
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
		return 0;
	app.SetTitle(string("FireCube Test Application"));	
	model=app.modelManager.Create("1.3ds");
	font=Application::GetContext().fontManager->Create("c:\\windows\\fonts\\arial.ttf:18");		
	app.Run();		
	return 0;
}
bool App::Update(float t)
{	
	
	return true;
}
bool App::Render(float t)
{		
	renderer.Clear(vec4(0.2f,0.2f,0.6f,1.0f),1.0f);
	renderer.SetPerspectiveProjection(90.0f,0.1f,100.0f);	
	mat4 m;	
	m.Translate(vec3(0,0,rot.z));
	m.RotateX(rot.x);
	m.RotateY(rot.y);	
	renderer.SetModelViewMatrix(m);		
	renderer.Render(model);
	renderer.SetModelViewMatrix(mat4());
	renderer.SetOrthographicProjection();
	
	ostringstream oss;
	oss << "FPS:"<<app.GetFps();	
	renderer.RenderText(font,vec2(0,0),oss.str());
	return true;
}
bool App::HandleInput(float t)
{
	POINT p;
	vec3 m;	
	GetCursorPos(&p);	
	m.x=(float)p.x;
	m.y=(float)p.y;	
	if (GetAsyncKeyState(1))
	{
		rot.x+=t*(lastPos.y-m.y)/2.0f;
		rot.y+=t*(lastPos.x-m.x)/2.0f;		
		
		rot.x-=rot.x>=2*PI ? (float)(2*PI) : 0;
		rot.x+=rot.x<0 ? (float)(2*PI) : 0;
		rot.y-=rot.y>=2*PI ? (float)(2*PI) : 0;
		rot.y+=rot.y<0 ? (float)(2*PI) : 0;				
	}
	if (GetAsyncKeyState(2))	
		rot.z+=t*(lastPos.y-m.y)*2.0f;

	lastPos=m;
	return true;
}
