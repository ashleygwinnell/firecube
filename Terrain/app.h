class App : public Application
{
public:
	bool Init();
	void HandleInput(float time);
	void Update(float time);
	void Render(float time);	
	
	Font font;
	Terrain terrain;
};