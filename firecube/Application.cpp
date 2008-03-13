#include <string>
#include <vector>
using namespace std;
#include <Windows.h>
#include <SDL/SDL.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;
Application::Application() : running(false), frameCount(0), fpsTime(0)
{
}
Application::~Application()
{
	Destroy();
}
bool Application::Initialize()
{
	SDL_Surface *screen;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)!=0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen=SDL_SetVideoMode(800,600,32,SDL_OPENGL);
	if (!screen)
		return false;
	
	timer.Init();
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepth(1.0f);
	glViewport(0,0,800,600);
	mat4 mat;
	mat.GeneratePerspective(90.0f,800.0f/600.0f,0.1f,100);	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(mat.m);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_CULL_FACE);

	return true;
}
bool Application::Destroy()
{
	SDL_Quit();
	return true;
}
void Application::Run()
{	
	running=true;
	SDL_Event event;
	while (running)
	{		
		deltaTime=(float)timer.Passed();		
		timer.Update();
		while(SDL_PollEvent(&event)) 
		{
			if (event.type==SDL_KEYDOWN)
			{
				if (event.key.keysym.sym==SDLK_ESCAPE)
				{
					running=false;
				}
			}
			if (event.type==SDL_QUIT) 
				running=false;
		}		
		HandleInput(deltaTime);
		Update(deltaTime);
		Render(deltaTime);
		SDL_GL_SwapBuffers();
		frameCount+=1.0f;
		fpsTime+=deltaTime;
		if (fpsTime>=1.0f)
		{
			fps=frameCount/fpsTime;
			fpsTime-=1.0f;
			frameCount=0;
		}
	}
}
void Application::SetTitle(string &title)
{
	SDL_WM_SetCaption(title.c_str(),NULL);
}
float Application::GetFps()
{
	return fps;
}