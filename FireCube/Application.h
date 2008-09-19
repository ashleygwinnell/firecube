#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(push)
#pragma warning(disable:4251)

/** 
* Defines an application context.<br>
* Contains pointers to various resource managers(textures,shaders,fonts).<br>
* The context is used by various resource loading functions.
*/
class FIRECUBE_API ApplicationContext
{
public:
	ApplicationContext();
	ApplicationContext(TextureManager *textureManager,ShaderManager *shaderManager,FontManager *fontManager);
	TextureManager *textureManager;
	ShaderManager *shaderManager;
	FontManager *fontManager;
};
/** 
This class is responsible for the intialization and running of the application.
*/
class FIRECUBE_API Application
{
public:
	Application();
	virtual ~Application();
	/** 
	* Creates a default window and intializes OpenGL.
	* @return true on success, false otherwise.
	*/
	bool Initialize();
	/** 
	* Creates a window and intializes OpenGL.
	* @return true on success, false otherwise.
	*/
	bool Initialize(int width,int height,int bpp,bool fullscreen);
	/** 
	* Destroy the window and it's associated OpenGL context.
	* @return true on success, false otherwise.
	*/
	bool InitializeNoWindow();
	/** 
	* Initializes without creating a window (assumes there is already an active rendering context).
	* @return true on success, false otherwise.
	*/
	bool Destroy();
	/** 
	* Enters the main rendering loop.
	*/
	void Run();
	/** 
	* Sets the title of the window.
	*/
	void SetTitle(string &title);
	/** 
	* Returns the current frames per second.
	*/
	float GetFps();
	/** 
	* Sets the current application context.
	*/
	static void SetContext(ApplicationContext &context);
	/** 
	* Returns the current application context.
	*/
	static ApplicationContext GetContext();
	/** 
	* User should override this to handle the input.
	* Called once per frame.
	*/
	virtual bool HandleInput(float time) = 0;
	/** 
	* User should override this to handle the game logic.
	* Called once per frame.
	*/
	virtual bool Update(float time) = 0;
	/** 
	* User should override this to render the game world.
	* Called once per frame.
	*/
	virtual bool Render(float time) = 0;
//private:
	Timer timer;	
	bool running;
	float deltaTime;;
	float fps;
	float fpsTime;
	float frameCount;
	ApplicationContext defaultContext;
	TextureManager defaultTextureManager;
	ShaderManager defaultShaderManager;
	FontManager defaultFontManager;
	static ApplicationContext *currentContext;
};
#pragma warning(pop)
#endif