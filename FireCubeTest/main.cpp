#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
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
Font font;
Font font2;
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
		return 0;
	app.SetTitle(string("FireCube Test Application"));
	glClearColor(0.2f,0.2f,0.6f,1.0f);	
	model=app.modelManager.Create("1.3ds");	
	font=app.GetContext().fontManager->Create("c:\\windows\\fonts\\Arial.ttf:18");
	font2=app.GetContext().fontManager->Create("c:\\windows\\fonts\\Arial.ttf:40");
	app.Run();		
	return 0;
}
bool App::Update(float t)
{	
	
	return true;
}
bool App::Render(float t)
{		
	renderer.Clear();
	mat4 p;
	p.GeneratePerspective(90,800.0f/600.0f,0.1f,100.0f);
	renderer.SetProjectionMatrix(p);
	mat4 m;	
	m.Translate(vec3(0,0,rot.z));
	m.RotateX(rot.x);
	m.RotateY(rot.y);	
	renderer.SetModelViewMatrix(m);		
	renderer.Render(model);
	p.Identity();
	renderer.SetModelViewMatrix(p);
	p.GenerateOrthographic(0,800.0f,600.0f,0,0,1);
	renderer.SetProjectionMatrix(p);	
	
	/*renderer.UseTexture(font->page->tex,0);
	float size=512;
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex2f(0,0);
	glTexCoord2f(0,0); glVertex2f(0,size);
	glTexCoord2f(1,0); glVertex2f(size,size);
	glTexCoord2f(1,1); glVertex2f(size,0);	
	glEnd();
	glEnable(GL_DEPTH_TEST);*/
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
