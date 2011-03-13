#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
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
	* Returns the cursor position.
	*/
	vec2 GetCursorPos();

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
    * Called once initialization is complete to execute user specific initialization.
    */
    virtual bool Init();

    /**
    * Add a search path for resources.
    * @param path The path to add.
    */
    static void AddSearchPath(const std::string &path);

    /**
    * Gets the list of search paths.
    * @return The list of search paths.
    */
    static const std::vector<std::string> &GetSearchPaths();

    /**
    * Searches for file name using the search paths.
    * @param filename The file name to search.
    */
    static std::string SearchForFileName(const std::string &filename);
private:	
    Timer timer;
    bool running;
    float deltaTime;
    float fps;
    float fpsTime;
    float frameCount;
    int width, height;
    TextureManager defaultTextureManager;
    ShaderManager defaultShaderManager;
    FontManager defaultFontManager;
    static std::vector<std::string> searchPaths;
};
}
#pragma warning(pop)
#endif