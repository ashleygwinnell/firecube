#include <SDL.h>
#include "ThirdParty/GLEW/glew.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Utils/Logger.h"
#include "Rendering/Renderer.h"
#include "Application/Application.h"
#include "Core/ResourceCache.h"
#include "Core/Engine.h"
#include "Rendering/DebugRenderer.h"
#include "Core/Events.h"

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
	return Initialize(800, 600, 0, false);
}

bool Application::Initialize(int width, int height, int multisample, bool fullscreen)
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
	resourceCache = new ResourceCache(engine);
	engine->SetResourceCache(resourceCache);
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
	delete resourceCache;
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
	timer.Update();
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
					KeyModifier modifier = KeyModifier::NONE;
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
					modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
					// Update the input manager's state
					inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
					
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				// Check for key modifiers
				SDL_Keymod keyMod = SDL_GetModState();
				KeyModifier modifier = KeyModifier::NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
				KeyModifier modifier = KeyModifier::NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
				KeyModifier modifier = KeyModifier::NONE;
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
				modifier = (KeyModifier) (modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
				inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_X_RELATIVE, (float)event.motion.xrel);
				inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_Y_RELATIVE, (float)event.motion.yrel);
				inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_X_ABSOLUTE, (float)event.motion.x);
				inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_Y_ABSOLUTE, (float)event.motion.y);
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{	
				// Set the corresponding analog values of the input manager (mouse wheel)
				inputManager.SetRawAnalogValue(AnalogInput::MOUSE_WHEEL_Y_RELATIVE, ((float)event.wheel.y) / 60.0f);
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
		inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_X_ABSOLUTE, (float)x);
		inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_Y_ABSOLUTE, (float)y);
		renderer->ResetNumberOfPrimitivesRendered();
		renderer->SetTimeStep(deltaTime);
		// Dispatch input to all input listeners
		inputManager.DispatchInput(deltaTime);
		// Update the scene
		Events::Update(deltaTime);
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
	keyMap[SDLK_a] = Key::A;
	keyMap[SDLK_b] = Key::B;
	keyMap[SDLK_c] = Key::C;
	keyMap[SDLK_d] = Key::D;
	keyMap[SDLK_e] = Key::E;
	keyMap[SDLK_f] = Key::F;
	keyMap[SDLK_g] = Key::G;
	keyMap[SDLK_h] = Key::H;
	keyMap[SDLK_i] = Key::I;
	keyMap[SDLK_j] = Key::J;
	keyMap[SDLK_k] = Key::K;
	keyMap[SDLK_l] = Key::L;
	keyMap[SDLK_m] = Key::M;
	keyMap[SDLK_n] = Key::N;
	keyMap[SDLK_o] = Key::O;
	keyMap[SDLK_p] = Key::P;
	keyMap[SDLK_q] = Key::Q;
	keyMap[SDLK_r] = Key::R;
	keyMap[SDLK_s] = Key::S;
	keyMap[SDLK_t] = Key::T;
	keyMap[SDLK_u] = Key::U;
	keyMap[SDLK_v] = Key::V;
	keyMap[SDLK_w] = Key::W;
	keyMap[SDLK_x] = Key::X;
	keyMap[SDLK_y] = Key::Y;
	keyMap[SDLK_z] = Key::Z;
	keyMap[SDLK_0] = Key::NUM0;
	keyMap[SDLK_1] = Key::NUM1;
	keyMap[SDLK_2] = Key::NUM2;
	keyMap[SDLK_3] = Key::NUM3;
	keyMap[SDLK_4] = Key::NUM4;
	keyMap[SDLK_5] = Key::NUM5;
	keyMap[SDLK_6] = Key::NUM6;
	keyMap[SDLK_7] = Key::NUM7;
	keyMap[SDLK_8] = Key::NUM8;
	keyMap[SDLK_9] = Key::NUM9;
	keyMap[SDLK_ESCAPE] = Key::ESCAPE;
	keyMap[SDLK_LEFT] = Key::LEFT;
	keyMap[SDLK_RIGHT] = Key::RIGHT;
	keyMap[SDLK_UP] = Key::UP;
	keyMap[SDLK_DOWN] = Key::DOWN;
	keyMap[SDLK_LSHIFT] = Key::LEFT_SHIFT;
	keyMap[SDLK_RSHIFT] = Key::RIGHT_SHIFT;
	keyMap[SDLK_LCTRL] = Key::LEFT_CTRL;
	keyMap[SDLK_RCTRL] = Key::RIGHT_CTRL;
	keyMap[SDLK_LALT] = Key::LEFT_ALT;
	keyMap[SDLK_RALT] = Key::RIGHT_ALT;
	keyMap[SDLK_TAB] = Key::TAB;
	keyMap[SDLK_SPACE] = Key::SPACE;
	keyMap[SDLK_F1] = Key::F1;
	keyMap[SDLK_F2] = Key::F2;
	keyMap[SDLK_F3] = Key::F3;
	keyMap[SDLK_F4] = Key::F4;
	keyMap[SDLK_F5] = Key::F5;
	keyMap[SDLK_F6] = Key::F6;
	keyMap[SDLK_F7] = Key::F7;
	keyMap[SDLK_F8] = Key::F8;
	keyMap[SDLK_F9] = Key::F9;
	keyMap[SDLK_F10] = Key::F10;
	keyMap[SDLK_F11] = Key::F11;
	keyMap[SDLK_F12] = Key::F12;
	mouseMap[1] = Key::MOUSE_LEFT_BUTTON;
	mouseMap[2] = Key::MOUSE_MIDDLE_BUTTON;
	mouseMap[3] = Key::MOUSE_RIGHT_BUTTON;
}

Engine *Application::GetEngine()
{
	return engine;
}

float Application::GetPassedTime() const
{
	return passedTime;
}
