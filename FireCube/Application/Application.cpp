#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <streambuf>
#include <set>
#include <io.h>
#include <fcntl.h>
using namespace std;
#include <Windows.h>
#include <SDL.h>
#include "Dependencies/GLee.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Utils/ResourceManager.h"
#include "Utils/Timer.h"
#include "Math/MyMath.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Application/Input.h"
#include "Application/Application.h"

using namespace FireCube;


extern void InitializeRenderer();
extern void DestroyRenderer();
extern void ResetNumberOfTrianglesRendered();
extern FT_Library freeTypeLibrary;

Application::Application() : running(false), frameCount(0), fpsTime(0), fps(0)
{
    Renderer::SetTextureManager(defaultTextureManager);
    Renderer::SetShaderManager(defaultShaderManager);
    Renderer::SetFontManager(defaultFontManager);
}

Application::~Application()
{
	DestroyRenderer();
	Logger::Write(Logger::LOG_INFO, string("Destroying application"));
	SDL_Quit();
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
    Renderer::SetModelViewMatrix(mat4::identity);
    this->width = width;
    this->height = height;
	InitKeyMap();
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
	running = false;  
    return true;
}

void Application::Run()
{
    running = true;
    SDL_Event event;
    Logger::Write(Logger::LOG_INFO, string("Entering main loop..."));
    while (running)
    {
        deltaTime = (float) timer.Passed();
        timer.Update();
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {              
				map<int, Key>::iterator k = keyMap.find(event.key.keysym.sym);
				if (k != keyMap.end())
				{
					KeyModifier modifier = MODIFIER_NONE;					
					map<int, bool>::iterator i = keyState.find(event.key.keysym.sym);
					bool previouslyPressed = i != keyState.end() && i->second == true;
					keyState[event.key.keysym.sym] = true;
					modifier = (KeyModifier) (modifier | (keyState[SDLK_LSHIFT] ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | (keyState[SDLK_RSHIFT] ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | (keyState[SDLK_LCTRL] ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | (keyState[SDLK_RCTRL] ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | (keyState[SDLK_LALT] ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | (keyState[SDLK_RALT] ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
					inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
					
				}
            }
			else if (event.type == SDL_KEYUP)
			{
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LSHIFT] ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RSHIFT] ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LCTRL] ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RCTRL] ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LALT] ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RALT] ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				map<int, Key>::iterator k = keyMap.find(event.key.keysym.sym);
				if (k != keyMap.end())
				{
					inputManager.SetRawKeyState(k->second, false, true, modifier);
					keyState[event.key.keysym.sym] = false;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LSHIFT] ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RSHIFT] ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LCTRL] ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RCTRL] ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LALT] ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RALT] ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				map<int, Key>::iterator k = mouseMap.find(event.button.button);
				if (k != mouseMap.end())
				{
					map<int, bool>::iterator i = mouseState.find(event.button.button);
					bool previouslyPressed = i != mouseState.end() && i->second == true;
					inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
					mouseState[event.button.button] = true;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LSHIFT] ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RSHIFT] ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LCTRL] ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RCTRL] ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_LALT] ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | (keyState[SDLK_RALT] ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				map<int, Key>::iterator k = mouseMap.find(event.button.button);
				if (k != mouseMap.end())
				{
					inputManager.SetRawKeyState(k->second, false, true, modifier);
					mouseState[event.button.button] = false;
				}
			}
            else if (event.type == SDL_MOUSEMOTION)
            {
				inputManager.SetRawAnalogValue(MOUSE_AXIS_X_RELATIVE, event.motion.xrel);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_Y_RELATIVE, event.motion.yrel);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_X_ABSOLUTE, event.motion.x);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_Y_ABSOLUTE, event.motion.y);
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
        ResetNumberOfTrianglesRendered();
        inputManager.DispatchInput(deltaTime);
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

InputManager &Application::GetInputManager()
{
	return inputManager;
}

void Application::InitKeyMap()
{
	keyMap[SDLK_a] = KEY_A;
	keyMap[SDLK_b] = KEY_B;
	keyMap[SDLK_c] = KEY_C;
	keyMap[SDLK_d] = KEY_D;
	keyMap[SDLK_e] = KEY_E;
	keyMap[SDLK_f] = KEY_F;
	keyMap[SDLK_g] = KEY_G;
	keyMap[SDLK_h] = KEY_H;
	keyMap[SDLK_i] = KEY_I;
	keyMap[SDLK_j] = KEY_J;
	keyMap[SDLK_k] = KEY_K;
	keyMap[SDLK_l] = KEY_L;
	keyMap[SDLK_m] = KEY_M;
	keyMap[SDLK_n] = KEY_N;
	keyMap[SDLK_o] = KEY_O;
	keyMap[SDLK_p] = KEY_P;
	keyMap[SDLK_q] = KEY_Q;
	keyMap[SDLK_r] = KEY_R;
	keyMap[SDLK_s] = KEY_S;
	keyMap[SDLK_t] = KEY_T;
	keyMap[SDLK_u] = KEY_U;
	keyMap[SDLK_v] = KEY_V;
	keyMap[SDLK_w] = KEY_W;
	keyMap[SDLK_x] = KEY_X;
	keyMap[SDLK_y] = KEY_Y;
	keyMap[SDLK_z] = KEY_Z;
	keyMap[SDLK_0] = KEY_0;
	keyMap[SDLK_1] = KEY_1;
	keyMap[SDLK_2] = KEY_2;
	keyMap[SDLK_3] = KEY_3;
	keyMap[SDLK_4] = KEY_4;
	keyMap[SDLK_5] = KEY_5;
	keyMap[SDLK_6] = KEY_6;
	keyMap[SDLK_7] = KEY_7;
	keyMap[SDLK_8] = KEY_8;
	keyMap[SDLK_9] = KEY_9;
	keyMap[SDLK_ESCAPE] = KEY_ESCAPE;
	keyMap[SDLK_LEFT] = KEY_LEFT;
	keyMap[SDLK_RIGHT] = KEY_RIGHT;
	keyMap[SDLK_UP] = KEY_UP;
	keyMap[SDLK_DOWN] = KEY_DOWN;
	keyMap[SDLK_LSHIFT] = KEY_LEFT_SHIFT;
	keyMap[SDLK_RSHIFT] = KEY_RIGHT_SHIFT;
	keyMap[SDLK_LCTRL] = KEY_LEFT_CTRL;
	keyMap[SDLK_RCTRL] = KEY_RIGHT_CTRL;
	keyMap[SDLK_LALT] = KEY_LEFT_ALT;
	keyMap[SDLK_RALT] = KEY_RIGHT_ALT;
	mouseMap[1] = KEY_MOUSE_LEFT_BUTTON;
	mouseMap[2] = KEY_MOUSE_MIDDLE_BUTTON;
	mouseMap[3] = KEY_MOUSE_RIGHT_BUTTON;
	mouseMap[4] = KEY_MOUSE_WHEEL_UP;
	mouseMap[5] = KEY_MOUSE_WHEEL_DOWN;
}