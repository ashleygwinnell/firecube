#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <streambuf>
#include <io.h>
#include <fcntl.h>
using namespace std;
#include <Windows.h>
#include <SDL/SDL.h>
#include "GLee.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "FireCube.h"
using namespace FireCube;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/object.hpp>

extern void InitializeRenderer();
extern void DestroyRenderer();
extern FT_Library freeTypeLibrary;

vector<string> Application::searchPaths;
bool (*Application::outputCallback)(string &str,bool out)=NULL;
lua_State *luaState=NULL;
void InitializeLua(lua_State *& luaState);
Application::Application() : running(false), frameCount(0), fpsTime(0), fps(0)
{
	Renderer::SetTextureManager(defaultTextureManager);
	Renderer::SetShaderManager(defaultShaderManager);
	Renderer::SetFontManager(defaultFontManager);
}
Application::~Application()
{
	Destroy();
}
bool Application::Initialize()
{	
	return Initialize(800,600,32,0,false);
}
bool Application::Initialize(int width,int height,int bpp,int multisample,bool fullscreen)
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
	if (multisample)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,multisample);
	}
	
	screen=SDL_SetVideoMode(width,height,bpp,SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE));
	if (!screen)
		return false;
		
	Renderer::SetViewport(0,0,width,height);	
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100);	
	Renderer::SetModelViewMatrix(mat4());
	this->width=width;
	this->height=height;
	return InitializeNoWindow();
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
	return Init();
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
				FireCube::Event evt;
				evt.type=KEY_DOWN;
				evt.c=event.key.keysym.scancode;
				eventQueue.push(evt);
				if (event.key.keysym.sym==SDLK_ESCAPE)
				{
					running=false;
				}
			}
			if (event.type==SDL_VIDEORESIZE)
			{
				width=event.resize.w;
				height=event.resize.h;
				Renderer::SetViewport(0,0,event.resize.w,event.resize.h);				
			}
			if (event.type==SDL_KEYUP)
			{
				FireCube::Event evt;
				evt.type=KEY_UP;
				evt.c=event.key.keysym.scancode;
				eventQueue.push(evt);
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
void Application::SetTitle(const string &title)
{
	SDL_WM_SetCaption(title.c_str(),NULL);
}
float Application::GetFps() const
{
	return fps;
}
bool Application::Init()
{
	return true;
}
int Application::GetWidth() const
{
	return width;
}
int Application::GetHeight() const
{
	return height;
}
bool Application::HasMoreEvents()
{
	return !eventQueue.empty();
}
Event Application::GetEvent()
{
	Event ret=eventQueue.front();
	eventQueue.pop();
	return ret;
}
void Application::AddSearchPath(const string &path)
{
	string npath=path;
	if ((npath[npath.size()-1]=='\\') || (npath[npath.size()-1]=='/'))
		npath=npath.substr(0,npath.size()-1);
	Application::searchPaths.push_back(npath);
}
const vector<string> &Application::GetSearchPaths()
{
	return Application::searchPaths;
}

/* CPPDOC_BEGIN_EXCLUDE */
DWORD WINAPI ReadIO(void *param)
{
	HANDLE rd=(HANDLE)param;
	char buf[100];
	while (true)
	{
		DWORD readBytes=0;
		if (!ReadFile(rd, buf, 99, &readBytes, 0))
			break;
		buf[99] = 0;
		if (readBytes>0)
		{			
			std::string str;
			for (DWORD i=0;i<readBytes;i++)
				str+=buf[i];	
			Application::GetOutputCallback()(str,true);
		}
		else
			break;
	}	
	return 0;
}
void RedirectConsole(HANDLE hPipe,int &hConHandle1,int &hConHandle2)
{
	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE *fp = 0 ;

	// redirect unbuffered STDOUT to the Pipe
	lStdHandle = hPipe;
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	hConHandle1=hConHandle;
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the Pipe
	lStdHandle = hPipe;
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	hConHandle2=hConHandle;
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
}
void RestoreConsole()
{
	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE *fp = 0 ;
	
	// redirect unbuffered STDOUT to the Console
	lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (lStdHandle)
	{	
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
	}

	// redirect unbuffered STDERR to the Console
	lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
	if (lStdHandle)
	{	
		hConHandle = _open_osfhandle(reinterpret_cast<intptr_t>(lStdHandle), _O_TEXT);
		fp = _fdopen(hConHandle, "w");
		*stderr = *fp;
		setvbuf(stderr, NULL, _IONBF, 0);
	}
}

/* CPPDOC_END_EXCLUDE */

void Application::ExecuteString(const string &str)
{
	if (Application::outputCallback!=NULL)
	{
		HANDLE rd,wr;
		int hConHandle1,hConHandle2;
		if (!CreatePipe(&rd,&wr,NULL,0))
			return;
		RedirectConsole(wr,hConHandle1,hConHandle2);
		HANDLE threadHandle=CreateThread(NULL,0,ReadIO,(LPVOID)rd,0,0);		
		luaL_dostring(luaState,str.c_str());
		WaitForSingleObject(threadHandle,50);						
		//TerminateThread(threadHandle,0);
		_close(hConHandle1);
		_close(hConHandle2);	
		CloseHandle(wr);
		CloseHandle(rd);
		RestoreConsole();
	}
	else
		luaL_dostring(luaState,str.c_str());
}
void Application::ExecuteFile(const string &filename)
{
	if (Application::outputCallback!=NULL)
	{
		HANDLE rd,wr;
		int hConHandle1,hConHandle2;
		if (!CreatePipe(&rd,&wr,NULL,0))
			return;
		RedirectConsole(wr,hConHandle1,hConHandle2);
		HANDLE threadHandle=CreateThread(NULL,0,ReadIO,(LPVOID)rd,0,0);
		luaL_dofile(luaState,filename.c_str());
		WaitForSingleObject(threadHandle,50);		
		//TerminateThread(threadHandle,0);
		_close(hConHandle1);
		_close(hConHandle2);
		CloseHandle(wr);
		CloseHandle(rd);
		RestoreConsole();		
	}
	else
		luaL_dofile(luaState,filename.c_str());
}
void Application::InitializeLua()
{
	if (luaState==NULL)
		::InitializeLua(luaState);
}
lua_State *Application::GetLuaState()
{
	return luaState;
}
void Application::CloseLua()
{
	if (luaState!=NULL)
		lua_close(luaState);
	luaState=NULL;
}
void Application::SetOutputCallback(bool (*outputCallback)(string &,bool))
{
	Application::outputCallback=outputCallback;
}
bool (*Application::GetOutputCallback())(string& ,bool)
{
	return Application::outputCallback;
}