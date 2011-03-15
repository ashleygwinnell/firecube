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
#include <SDL.h>
#include "GLee.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "utils.h"
#include "Logger.h"
#include "Filesystem.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"
#include "Font.h"
#include "Renderer.h"
#include "Application.h"

using namespace FireCube;


extern void InitializeRenderer();
extern void DestroyRenderer();
extern FT_Library freeTypeLibrary;

Application::Application() : running(false), frameCount(0), fpsTime(0), fps(0)
{
    Renderer::SetTextureManager(defaultTextureManager);
    Renderer::SetShaderManager(defaultShaderManager);
    Renderer::SetFontManager(defaultFontManager);
}
Application::~Application()
{
    Close();
}
bool Application::Initialize()
{
    return Initialize(800, 600, 32, 0, false);
}
bool Application::Initialize(int width, int height, int bpp, int multisample, bool fullscreen)
{
    SDL_Surface *screen;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return false;
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if (multisample)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisample);
    }

    screen = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | (fullscreen ? SDL_FULLSCREEN : SDL_RESIZABLE));
    if (!screen)
        return false;

    Renderer::SetViewport(0, 0, width, height);
    Renderer::SetPerspectiveProjection(90.0f, 0.1f, 100);
    Renderer::SetModelViewMatrix(mat4());
    this->width = width;
    this->height = height;
    return InitializeNoWindow();
}
bool Application::InitializeNoWindow()
{
    Logger::Init("log.txt");
    Logger::Write(Logger::LOG_INFO, string("Initializing application"));
    timer.Init();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    srand(GetTickCount());
    FT_Init_FreeType(&freeTypeLibrary);
    InitializeRenderer();
    return Init();
}
bool Application::Close()
{
    DestroyRenderer();
    Logger::Write(Logger::LOG_INFO, string("Destroying application"));
    SDL_Quit();
    return true;
}
void Application::Run()
{
    running = true;
    SDL_Event event;
    Logger::Write(Logger::LOG_INFO, string("Entering main loop..."));
    while (running)
    {
        deltaTime = (float)timer.Passed();
        timer.Update();
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
			else if (event.type == SDL_MOUSEMOTION)
			{

			}
            else if (event.type == SDL_VIDEORESIZE)
            {
                width = event.resize.w;
                height = event.resize.h;
                Renderer::SetViewport(0, 0, event.resize.w, event.resize.h);
            }
            else if (event.type == SDL_QUIT)
                running = false;
        }
        HandleInput(deltaTime);
        Update(deltaTime);
        Render(deltaTime);
        SDL_GL_SwapBuffers();
        frameCount += 1.0f;
        fpsTime += deltaTime;
        if (fpsTime >= 1.0f)
        {
            fps = frameCount / fpsTime;
            fpsTime -= 1.0f;
            frameCount = 0;
        }
    }
    Logger::Write(Logger::LOG_INFO, string("Exiting main loop..."));
}
void Application::SetTitle(const string &title)
{
    SDL_WM_SetCaption(title.c_str(), NULL);
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
vec2 Application::GetCursorPos()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return vec2((float) x, (float) y);
}
