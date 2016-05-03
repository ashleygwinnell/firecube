#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <string>

#include "Core/Object.h"
#include "Application/Input.h"
#include "Utils/Timer.h"

struct SDL_Window;
typedef void *SDL_GLContext;
union SDL_Event;

namespace FireCube
{

// Forward declarations.
class Shader;
class Texture;
class Renderer;
class ResourceCache;
class DebugRenderer;
class UI;
class LuaState;
class Audio;

/**
* This class is responsible for the initialization and running of the application.
*/
class FIRECUBE_API Application : public Object
{
	FIRECUBE_OBJECT(Application);
public:
	Application();
	virtual ~Application();

	/**
	* Creates a default window and initializes OpenGL.
	* @return true on success, false otherwise.
	*/
	bool Initialize();

	/**
	* Creates a window and initializes OpenGL.
	* @return true on success, false otherwise.
	*/
	bool Initialize(int width, int height,int multisample, bool fullscreen, bool maximized);

	/**
	* Initializes without creating a window (assumes there is already an active rendering context).
	* @return true on success, false otherwise.
	*/
	bool InitializeNoWindow();

	/**
	* Destroys allocated resource by the application. Must be called after the application's main loop has returned.
	*/
	void Destroy();

	/**
	* Closes the application.
	* @return true on success, false otherwise.
	*/
	bool Close();

	/**
	* Enters the main rendering loop.
	*/
	void Run();

	/**
	* Sets the title of the window.
	*/
	void SetTitle(const std::string &title);

	/**
	* Returns the current frames per second.
	*/
	float GetFps() const;

	/**
	* Returns the window's width.
	*/
	int GetWidth() const;

	/**
	* Returns the window's height.
	*/
	int GetHeight() const;	
	
	/**
	* User should override this to handle the game logic.
	* Called once per frame.
	*/
	virtual void Update(float time) = 0;

	/**
	* User should override this to render the game world.
	* Called once per frame.
	*/
	virtual void Render(float time) = 0;

	/**
	* Called once initialization is complete to execute user specific initialization.
	*/
	virtual bool Prepare();
	
	/**
	* Returns the input manager.
	*/
	InputManager &GetInputManager();

	Engine *GetEngine();

	/**
	* Returns the passed time since the application was started in seconds.
	*/
	float GetPassedTime() const;

protected:
	Renderer *renderer;	
	ResourceCache *resourceCache;
	DebugRenderer *debugRenderer;
	UI *ui;
	LuaState *luaState;
	Audio *audio;
private:
	void InitKeyMap();
	void ProcessInput(const SDL_Event &event);

	std::map<int, Key> keyMap;
	std::map<int, bool> keyState;
	std::map<int, Key> mouseMap;
	std::map<int, bool> mouseState;
	Timer timer;
	bool running;
	float deltaTime;
	float fps;
	float fpsTime;
	float frameCount;
	float passedTime;
	int width, height;
	InputManager inputManager;
	bool appInitialized;

	SDL_Window *mainWindow;
	SDL_GLContext *context;
};
}
#pragma warning(pop)
