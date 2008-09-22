#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <Windows.h>
#include <SDL/SDL.h>
#include "GLee.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "FireCube.h"
using namespace FireCube;

extern void InitializeRenderer();
extern void DestroyRenderer();
extern FT_Library freeTypeLibrary;
Application::Application() : running(false), frameCount(0), fpsTime(0)
{
	Renderer::SetTextureManager(&defaultTextureManager);
	Renderer::SetShaderManager(&defaultShaderManager);
	Renderer::SetFontManager(&defaultFontManager);
}
Application::~Application()
{
	Destroy();
}
bool Application::Initialize()
{	
	return Initialize(800,600,32,false);
}
bool Application::Initialize(int width,int height,int bpp,bool fullscreen)
{
	SDL_Surface *screen;
	if (SDL_Init(SDL_INIT_VIDEO)!=0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	
	
	screen=SDL_SetVideoMode(width,height,bpp,SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : 0));
	if (!screen)
		return false;
		
	glViewport(0,0,width,height);
	mat4 mat;
	mat.GeneratePerspective(90.0f,(float)width/(float)height,0.1f,100);	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(mat.m);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	InitializeNoWindow();

	return true;
}
bool Application::InitializeNoWindow()
{	
	Logger::Init("log.txt");
	Logger::Write(string("Initializing application.\n"));	
	timer.Init();
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_CULL_FACE);
	srand(GetTickCount());
	FT_Init_FreeType(&freeTypeLibrary);	
	InitializeRenderer();	
	return true;
}
bool Application::Destroy()
{		
	DestroyRenderer();	
	Logger::Write(string("Destroying application.\n"));	
	SDL_Quit();	
	return true;
}
void Application::Run()
{	
	running=true;
	SDL_Event event;	
	Logger::Write(string("Entering main loop...\n"));
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
	Logger::Write(string("Exiting main loop...\n"));
}
void Application::SetTitle(string &title)
{
	SDL_WM_SetCaption(title.c_str(),NULL);
}
float Application::GetFps()
{
	return fps;
}