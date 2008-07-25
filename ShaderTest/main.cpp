#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <windows.h>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
Model model;
Renderer renderer;
vec3 lastPos;
vec3 rot(0,0,-3);
Shader vShader,pShader;
Program program;
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;
	vShader=app.GetContext().shaderManager->Create("v.vshader");
	pShader=app.GetContext().shaderManager->Create("p.pshader");
	program.Create();
	program.Attach(vShader);
	program.Attach(pShader);
	program.Link();
	glClearColor(0.2f,0.2f,0.6f,1.0f);
	model=app.modelManager.Create("teapot2.3ds");
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
	static float appTime=0;
	appTime+=t;
	renderer.Clear();
	mat4 m;
	m.Translate(vec3(0,0,rot.z));
	m.RotateX(rot.x);
	m.RotateY(rot.y);
	renderer.SetModelViewMatrix(m);
	program.Use();
	program.Uniform1i("tex0",0);
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
