#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(push)
#pragma warning(disable:4251)

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
	* Initializes without creating a window (assumes there is already an active rendering context).
	* @return true on success, false otherwise.
	*/
	bool InitializeNoWindow();
	/** 
	* Destroy the window and it's associated OpenGL context.
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
	void SetTitle(const string &title);
	/** 
	* Returns the current frames per second.
	*/
	float GetFps();	
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
	/**
	* Called once initialization is complete to execute user specific intialization.
	*/
	virtual bool Init();
//private:
	Timer timer;	
	bool running;
	float deltaTime;;
	float fps;
	float fpsTime;
	float frameCount;	
	TextureManager defaultTextureManager;
	ShaderManager defaultShaderManager;
	FontManager defaultFontManager;	
};
#pragma warning(pop)
#endif