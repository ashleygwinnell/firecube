#include <SDL.h>
#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/Renderer.h"
#include "Application/Application.h"
#include "Core/ResourceCache.h"
#include "Core/Engine.h"
#include "Rendering/DebugRenderer.h"
#include "Core/Events.h"
#include "UI/UI.h"
#include "Rendering/Font.h"
#include "Scripting/LuaState.h"
#include "Audio/Audio.h"

using namespace FireCube;

float Application::timeScale = 1.0f;

Application::Application() : Object(new Engine), running(false), frameCount(0), fpsTime(0), fps(0), context(nullptr), mainWindow(nullptr), appInitialized(false), passedTime(0.0f)
{
	
}

Application::~Application()
{
	Destroy();	
}

bool Application::Initialize(WindowProperties windowPrperties)
{    
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
		return false;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	if (windowPrperties.multisample)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, windowPrperties.multisample);
	}
	mainWindow = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowPrperties.width, windowPrperties.height, (windowPrperties.resizable ? SDL_WINDOW_RESIZABLE : 0) | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (windowPrperties.maximized ? SDL_WINDOW_MAXIMIZED : 0) | (windowPrperties.fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	
	if (!mainWindow)
		return false;
	
	context = new SDL_GLContext;
	*context = SDL_GL_CreateContext(mainWindow);		
	this->width = windowPrperties.width;
	this->height = windowPrperties.height;

	InitGameControllers();

	return InitializeNoWindow();
}

bool Application::InitializeNoWindow()
{	
	appInitialized = true;
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
	audio = new Audio();
	audio->Init();
	engine->SetAudio(audio);
	InitKeyMap();	
	glewExperimental = GL_TRUE;
	glewInit();
	ui = new UI(engine);
	engine->SetUI(ui);
	luaState = new LuaState(engine);
	engine->SetLuaState(luaState);
	Logger::Init("log.txt");
	LOGINFO("Initializing application");	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	srand(GetTickCount());
	Font::Init();
	
	renderer->Initialize();
	debugRenderer->Initialize();
	return Prepare();
}

void Application::Destroy()
{
	if (!appInitialized)
	{
		delete engine;
		return;
	}

	LOGINFO("Destroying application");
	delete resourceCache;
	renderer->Destroy();
	delete renderer;
	delete debugRenderer;
	delete ui;		
	delete audio;
	delete luaState;

	if (context)
	{
		SDL_GL_DeleteContext(*context);
	}
	if (mainWindow)
	{
		SDL_DestroyWindow(mainWindow);
	}

	if (mainWindow)
	{
		SDL_Quit();
	}

	delete engine;
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
		deltaTime = (float) timer.Passed() * Application::timeScale;
		passedTime += deltaTime;
		timer.Update();
		inputManager.ResetInputState();
		while(SDL_PollEvent(&event))
		{
			HandleSDLEvent(event);
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION 
				|| event.type == SDL_MOUSEWHEEL || event.type == SDL_CONTROLLERAXISMOTION || event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERBUTTONUP)
			{
				ProcessInput(event);
			}
			else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				// Update the renderer's viewport with the new window size
				width = event.window.data1;
				height = event.window.data2;
				renderer->SetWidth(width);
				renderer->SetHeight(height);				
			}
			else if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_CONTROLLERDEVICEADDED)
			{
				AddGameController(event.cdevice.which);
			}
			else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
			{
				RemoveGameController(event.cdevice.which);
			}
		}		
		int x,y;
		SDL_GetMouseState(&x, &y);
		inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_X_ABSOLUTE, (float)x);
		inputManager.SetRawAnalogValue(AnalogInput::MOUSE_AXIS_Y_ABSOLUTE, (float)y);		
		renderer->SetTimeStep(deltaTime);
		renderer->SetTime(GetPassedTime());
		// Dispatch input to all input listeners		
		Events::HandleInput(this, deltaTime, inputManager.GetMappedInput());
		// Update the scene
		Events::Update(this, deltaTime);
		Update(deltaTime);
		// Render the scene
		renderer->ResetNumberOfPrimitivesRendered();		
		renderer->Render();
		Render(deltaTime);
		// Render UI
		ui->Render(renderer);
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
	keyMap[SDLK_RETURN] = Key::RETURN;
	keyMap[SDLK_DELETE] = Key::DELETE;
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
	gameControllerMap[SDL_CONTROLLER_BUTTON_A] = Key::GAME_CONTROLLER_BUTTON_A;
	gameControllerMap[SDL_CONTROLLER_BUTTON_B] = Key::GAME_CONTROLLER_BUTTON_B;
	gameControllerMap[SDL_CONTROLLER_BUTTON_X] = Key::GAME_CONTROLLER_BUTTON_X;
	gameControllerMap[SDL_CONTROLLER_BUTTON_Y] = Key::GAME_CONTROLLER_BUTTON_Y;
	gameControllerMap[SDL_CONTROLLER_BUTTON_BACK] = Key::GAME_CONTROLLER_BUTTON_BACK;
	gameControllerMap[SDL_CONTROLLER_BUTTON_GUIDE] = Key::GAME_CONTROLLER_BUTTON_GUIDE;
	gameControllerMap[SDL_CONTROLLER_BUTTON_START] = Key::GAME_CONTROLLER_BUTTON_START;
	gameControllerMap[SDL_CONTROLLER_BUTTON_LEFTSTICK] = Key::GAME_CONTROLLER_BUTTON_LEFT_STICK;
	gameControllerMap[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = Key::GAME_CONTROLLER_BUTTON_RIGHT_STICK;
	gameControllerMap[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = Key::GAME_CONTROLLER_BUTTON_LEFT_SHOULDER;
	gameControllerMap[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = Key::GAME_CONTROLLER_BUTTON_RIGHT_SHOULDER;
	gameControllerMap[SDL_CONTROLLER_BUTTON_DPAD_UP] = Key::GAME_CONTROLLER_BUTTON_DPAD_UP;
	gameControllerMap[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = Key::GAME_CONTROLLER_BUTTON_DPAD_DOWN;
	gameControllerMap[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = Key::GAME_CONTROLLER_BUTTON_DPAD_LEFT;
	gameControllerMap[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = Key::GAME_CONTROLLER_BUTTON_DPAD_RIGHT;
}

Engine *Application::GetEngine()
{
	return engine;
}

float Application::GetPassedTime() const
{
	return passedTime;
}

void Application::ProcessInput(const SDL_Event &event)
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
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
			modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
			// Update the input manager's state
			inputManager.SetRawKeyState(k->second, true, previouslyPressed, modifier);
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		// Check for key modifiers
		SDL_Keymod keyMod = SDL_GetModState();
		KeyModifier modifier = KeyModifier::NONE;
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LSHIFT) ? KeyModifier::LEFT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RSHIFT) ? KeyModifier::RIGHT_SHIFT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LCTRL) ? KeyModifier::LEFT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RCTRL) ? KeyModifier::RIGHT_CTRL : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_LALT) ? KeyModifier::LEFT_ALT : KeyModifier::NONE));
		modifier = (KeyModifier)(modifier | ((keyMod & KMOD_RALT) ? KeyModifier::RIGHT_ALT : KeyModifier::NONE));
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
	else if (event.type == SDL_CONTROLLERAXISMOTION)
	{
		switch (event.caxis.axis)
		{
		case SDL_CONTROLLER_AXIS_LEFTX:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_LEFT_X, (float)event.caxis.value / 32768.0f);
			break;
		case SDL_CONTROLLER_AXIS_RIGHTX:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_RIGHT_X, (float)event.caxis.value / 32768.0f);
			break;
		case SDL_CONTROLLER_AXIS_LEFTY:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_LEFT_Y, (float)event.caxis.value / 32768.0f);
			break;
		case SDL_CONTROLLER_AXIS_RIGHTY:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_RIGHT_Y, (float)event.caxis.value / 32768.0f);
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_TRIGGER_LEFT, (float)event.caxis.value / 32768.0f);
			break;
		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			inputManager.SetRawAnalogValue(AnalogInput::GAME_CONTROLLER_TRIGGER_RIGHT, (float)event.caxis.value / 32768.0f);
			break;
		default:
			break;
		}
	}
	else if (event.type == SDL_CONTROLLERBUTTONDOWN)
	{
		// Get the mapping between SDL button code and the engine's key enum
		std::map<int, Key>::iterator k = gameControllerMap.find(event.cbutton.button);
		if (k != gameControllerMap.end())
		{
			// Check if previously pressed and update the game controller state
			std::map<int, bool>::iterator i = gameControllerState.find(event.cbutton.button);
			bool previouslyPressed = i != gameControllerState.end() && i->second == true;
			gameControllerState[event.cbutton.button] = true;
			// Update the input manager's state
			inputManager.SetRawKeyState(k->second, true, previouslyPressed, KeyModifier::NONE);
		}
	}
	else if (event.type == SDL_CONTROLLERBUTTONUP)
	{
		// Get the mapping between SDL button code and the engine's key enum
		std::map<int, Key>::iterator k = gameControllerMap.find(event.cbutton.button);
		if (k != gameControllerMap.end())
		{
			// Update the input manager's state
			inputManager.SetRawKeyState(k->second, false, true, KeyModifier::NONE);
			// Update the game controller state
			gameControllerState[event.cbutton.button] = false;
		}
	}
}

void Application::SetTimeScale(float timeScale)
{
	Application::timeScale = timeScale;
}

SDL_Window *Application::GetWindow() const
{
	return mainWindow;
}

void Application::InitGameControllers()
{
	int numJoysticks = SDL_NumJoysticks();
	for (int i = 0; i < numJoysticks; ++i)
	{
		if (SDL_IsGameController(i))
		{
			AddGameController(i);
		}
	}
}

void Application::AddGameController(int deviceId)
{
	GameControllerState state;
	state.gameController = SDL_GameControllerOpen(deviceId);
	SDL_Joystick *joystickHandle = SDL_GameControllerGetJoystick(state.gameController);
	state.joystickId = SDL_JoystickInstanceID(joystickHandle);

	if (SDL_JoystickIsHaptic(joystickHandle))
	{
		state.haptic = SDL_HapticOpenFromJoystick(joystickHandle);
		if (SDL_HapticRumbleSupported(state.haptic))
		{
			if (SDL_HapticRumbleInit(state.haptic))
			{
				SDL_HapticClose(state.haptic);
				state.haptic = nullptr;
			}
		}
	}
	gameControllers.push_back(state);
}

void Application::RemoveGameController(int joystickId)
{
	auto newEnd = std::remove_if(gameControllers.begin(), gameControllers.end(), [joystickId](const GameControllerState &state) {
		return state.joystickId == joystickId;
	});

	gameControllers.erase(newEnd, gameControllers.end());
}

