#pragma once

//#pragma warning(push)
//#pragma warning(disable:4251)

#include <string>

#include "Utils/ResourcePool.h"
#include "Application/Input.h"
#include "Utils/Timer.h"
#include "Rendering/Font.h"

struct SDL_Window;
typedef void *SDL_GLContext;

namespace FireCube
{

// Forward declarations.
class Shader;
typedef ResourcePool<Shader> ShaderPool;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
typedef ResourcePool<Texture> TexturePool;

/**
* This class is responsible for the initialization and running of the application.
*/
class FIRECUBE_API Application
{
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
	bool Initialize(int width, int height, int bpp, int multisample, bool fullscreen);

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
	virtual bool Init();
	
	/**
	* Returns the input manager.
	*/

	InputManager &GetInputManager();

private:
	void InitKeyMap();
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
	int width, height;
	TexturePool defaultTexturePool;
	ShaderPool defaultShaderPool;
	FontPool defaultFontPool;
	InputManager inputManager;

	SDL_Window *mainWindow;
	SDL_GLContext *context;
};
}
//#pragma warning(pop)
