#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(push)
#pragma warning(disable:4251)

enum EventType
{
	MOUSE_MOVE,MOUSE_CLICK,KEY_PRESSED,KEY_DOWN,KEY_UP
};
enum Key
{
	KEY_NONE = 0,
	KEY_UP_ARROW,
	KEY_DOWN_ARROW,
	KEY_LEFT_ARROW,
	KEY_RIGHT_ARROW,
	KEY_SPACE,
	KEY_LAST
};
class FIRECUBE_API Event
{
public:
	EventType type;
	int mouseX,mouseY;
	Key key;
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
	bool Initialize(int width,int height,int bpp,int multisample,bool fullscreen);
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
	* User should override this to handle the input.
	* Called once per frame.
	*/
	virtual void HandleInput(float time) = 0;
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
	* Called once initialization is complete to execute user specific intialization.
	*/
	virtual bool Init();
	/**
	* @return The next event in the event queue.
	*/
	Event GetEvent();
	/**
	* @return Whether there are more events pending.
	*/
	bool HasMoreEvents();
	/**
	* Add a search path for resources.
	* @param path The path to add.
	*/
	static void AddSearchPath(const string &path);
	/**
	* @return The list of search paths.
	*/
	static const vector<string> &GetSearchPaths();

	/**
	* Executes a lua string.
	* @param str The string to execute.
	*/
	static void ExecuteString(const string &str);
	/**
	* Executes a lua file.
	* @param filename The file to execute.
	*/
	static void ExecuteFile(const string &filename);
	/**
	* Initializes the lua state.
	*/
	static void InitializeLua();
	/**
	* closes the lua state.
	*/
	static void CloseLua();
	/**
	* @return The lua state.
	*/
	static lua_State *GetLuaState();
	/**
	* Sets the output callback to redirect console output.
	*/
	static void SetOutputCallback(bool (*outputCallback)(string& ,bool));
	/**
	* @return The output callback function.
	*/
	static bool (*GetOutputCallback())(string& ,bool);
private:
	Timer timer;	
	bool running;
	float deltaTime;
	float fps;
	float fpsTime;
	float frameCount;	
	int width,height;
	TextureManager defaultTextureManager;
	ShaderManager defaultShaderManager;
	FontManager defaultFontManager;
	static vector<string> searchPaths;	
	static bool (*outputCallback)(string &str,bool out);
	queue<Event> eventQueue;
};
#pragma warning(pop)
#endif