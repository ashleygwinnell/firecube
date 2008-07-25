#ifndef APPLICATION_H
#define APPLICATION_H

#pragma warning(push)
#pragma warning(disable:4251)

class FIRECUBE_API ApplicationContext
{
public:
	ApplicationContext();
	ApplicationContext(TextureManager *textureManager,ShaderManager *shaderManager);
	TextureManager *textureManager;
	ShaderManager *shaderManager;
};
class FIRECUBE_API Application
{
public:
	Application();
	virtual ~Application();

	bool Initialize();
	bool Destroy();
	void Run();
	void SetTitle(string &title);
	float GetFps();
	static void SetContext(ApplicationContext &context);
	static ApplicationContext GetContext();
	virtual bool HandleInput(float time) = 0;
	virtual bool Update(float time) = 0;
	virtual bool Render(float time) = 0;
private:
	Timer timer;	
	bool running;
	float deltaTime;;
	float fps;
	float fpsTime;
	float frameCount;
	ApplicationContext defaultContext;
	TextureManager defaultTextureManager;
	ShaderManager defaultShaderManager;
	static ApplicationContext *currentContext;
};
#pragma warning(pop)
#endif