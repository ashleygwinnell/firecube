#ifndef APPLICATION_H
#define APPLICATION_H
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
};
#endif