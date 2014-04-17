#include <SDL.h>
#include "ThirdParty/GLEW/glew.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Utils/Logger.h"
#include "Rendering/Renderer.h"
#include "Application/Application.h"
#include "Core/ResourcePool.h"
#include "Core/Engine.h"
#include "Rendering/DebugRenderer.h"

using namespace FireCube;

extern FT_Library freeTypeLibrary;

Application::Application() : engine(new Engine), running(false), frameCount(0), fpsTime(0), fps(0), context(nullptr), mainWindow(nullptr)
{
	
}

Application::~Application()
{
	
}

bool Application::Initialize()
{
	return Initialize(800, 600, 32, 0, false);
}

bool Application::Initialize(int width, int height, int bpp, int multisample, bool fullscreen)
{    
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
	mainWindow = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	
	if (!mainWindow)
		return false;
	
	context = new SDL_GLContext;
	*context = SDL_GL_CreateContext(mainWindow);		
	this->width = width;
	this->height = height;	
	return InitializeNoWindow();
}

bool Application::InitializeNoWindow()
{	
	renderer = new Renderer(engine);
	engine->SetRenderer(renderer);
	resourcePool = new ResourcePool(engine);
	engine->SetResourcePool(resourcePool);
	engine->SetInputManager(&inputManager);
	debugRenderer = new DebugRenderer(engine);
	engine->SetDebugRenderer(debugRenderer);
	renderer->SetWidth(width);
	renderer->SetHeight(height);
	renderer->SetViewport(0, 0, width, height);
	InitKeyMap();	
	glewExperimental = GL_TRUE;
	glewInit();
	Logger::Init("log.txt");
	LOGINFO("Initializing application");	
	timer.Init();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	srand(GetTickCount());
	FT_Init_FreeType(&freeTypeLibrary);
	renderer->Initialize();
	debugRenderer->Initialize();
	return Prepare();
}

void Application::Destroy()
{
	delete resourcePool;
	renderer->Destroy();
	delete renderer;
	delete debugRenderer;
	LOGINFO("Destroying application");
	if (context)
		SDL_GL_DeleteContext(*context);
	if (mainWindow)
		SDL_DestroyWindow(mainWindow);	
	SDL_Quit();
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
	LOGINFO("Entering main loop...");
	while (running)
	{
		// Get time passed since last frame
		deltaTime = (float) timer.Passed();
		passedTime += deltaTime;
		timer.Update();
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{		
				// Get the mapping between SDL key code and the engine's key enum
				std::map<int, Key>::iterator k = keyMap.find(event.key.keysym.sym);
				if (k != keyMap.end())
				{					
					// Check if previously pressed and update the key state
					std::map<int, bool>::iterator i = keyState.find(event.key.keysym.sym);
					bool previouslyPressed = i != keyState.end() && i->second == true;
					keyState[event.key.keysym.sym] = true;
					SDL_Keymod keyMod = SDL_GetModState();
					// Check for key modifiers
					KeyModifier modifier = MODIFIER_NONE;
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
					// Update the input manager's state
					inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
					
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				// Check for key modifiers
				SDL_Keymod keyMod = SDL_GetModState();
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				// Get the mapping between SDL key code and the engine's key enum
				std::map<int, Key>::iterator k = keyMap.find(event.key.keysym.sym);
				if (k != keyMap.end())
				{
					// Update the input manager's state
					inputManager.SetRawKeyState(k->second, false, true, modifier);
					// Update the key state
					keyState[event.key.keysym.sym] = false;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{					
				// Check for key modifiers
				SDL_Keymod keyMod = SDL_GetModState();
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				// Get the mapping between SDL key code and the engine's key enum
				std::map<int, Key>::iterator k = mouseMap.find(event.button.button);
				if (k != mouseMap.end())
				{
					// Check if previously pressed and update the key state
					std::map<int, bool>::iterator i = mouseState.find(event.button.button);
					bool previouslyPressed = i != mouseState.end() && i->second == true;
					// Update the input manager's state
					inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
					mouseState[event.button.button] = true;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				// Check for key modifiers
				SDL_Keymod keyMod = SDL_GetModState();
				KeyModifier modifier = MODIFIER_NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? MODIFIER_LEFT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? MODIFIER_RIGHT_SHIFT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? MODIFIER_LEFT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? MODIFIER_RIGHT_CTRL : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? MODIFIER_LEFT_ALT : MODIFIER_NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? MODIFIER_RIGHT_ALT : MODIFIER_NONE));
				// Get the mapping between SDL key code and the engine's key enum
				std::map<int, Key>::iterator k = mouseMap.find(event.button.button);
				if (k != mouseMap.end())
				{
					// Update the input manager's state
					inputManager.SetRawKeyState(k->second, false, true, modifier);
					// Update the key state
					mouseState[event.button.button] = false;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				// Set the corresponding analog values of the input manager (mouse location)
				inputManager.SetRawAnalogValue(MOUSE_AXIS_X_RELATIVE, (float) event.motion.xrel);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_Y_RELATIVE, (float) event.motion.yrel);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_X_ABSOLUTE, (float) event.motion.x);
				inputManager.SetRawAnalogValue(MOUSE_AXIS_Y_ABSOLUTE, (float) event.motion.y);
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{	
				// Set the corresponding analog values of the input manager (mouse wheel)
				inputManager.SetRawAnalogValue(MOUSE_WHEEL_Y_RELATIVE, ((float) event.wheel.y) / 60.0f);
			}
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				// Update the renderer's viewport with the new window size
				width = event.window.data1;
				height = event.window.data2;
				renderer->SetWidth(width);
				renderer->SetHeight(height);
				renderer->SetViewport(0, 0, width, height);
			}
			else if (event.type == SDL_QUIT)
				running = false;
		}		
		int x,y;
		SDL_GetMouseState(&x, &y);
		inputManager.SetRawAnalogValue(MOUSE_AXIS_X_ABSOLUTE, (float) x);
		inputManager.SetRawAnalogValue(MOUSE_AXIS_Y_ABSOLUTE, (float) y);
		renderer->ResetNumberOfPrimitivesRendered();
		// Dispatch input to all input listeners
		inputManager.DispatchInput(deltaTime);
		// Update the scene
		Update(deltaTime);
		// Render the scene
		Render(deltaTime);
		SDL_GL_SwapWindow(mainWindow);
		frameCount += 1.0f;
		fpsTime += deltaTime;
		if (fpsTime >= 1.0f)
		{
			fps = frameCount / fpsTime;
			fpsTime = 0.0f;
			frameCount = 0;
		}
	}
	LOGINFO("Exiting main loop...");
	Destroy();
}

void Application::SetTitle(const std::string &title)
{
	SDL_SetWindowTitle(mainWindow, title.c_str());
}

float Application::GetFps() const
{
	return fps;
}

bool Application::Prepare()
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
	keyMap[SDLK_TAB] = KEY_TAB;
	keyMap[SDLK_SPACE] = KEY_SPACE;
	mouseMap[1] = KEY_MOUSE_LEFT_BUTTON;
	mouseMap[2] = KEY_MOUSE_MIDDLE_BUTTON;
	mouseMap[3] = KEY_MOUSE_RIGHT_BUTTON;
}

Engine *Application::GetEngine()
{
	return engine;
}

float Application::GetPassedTime() const
{
	return passedTime;
}
