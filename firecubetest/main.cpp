#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
Renderer renderer;
Model model;
vec3 lastPos;
vec3 rot(0,0,-3);
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{	
	if (!app.Initialize())
		return 0;
	model.Load3ds("1.3ds","Box01");			
	app.Run();	
	return 0;
}
bool App::Update(float t)
{
	ostringstream ss;
	ss << "FireCube Test Application FPS:"<<app.GetFps();
	app.SetTitle(ss.str());
	return true;
}
bool App::Render(float t)
{
	renderer.Clear();
	mat4 m;
	m.Translate(vec3(0,0,rot.z));
	m.RotateX(rot.x);
	m.RotateY(rot.y);
	renderer.SetModelViewMatrix(m);		
	renderer.Render(model);
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
