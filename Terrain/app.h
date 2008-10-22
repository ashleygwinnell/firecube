class App : public Application
{
public:
	bool Init();
	void HandleInput(float time);
	void Update(float time);
	void Render(float time);		
		
	QuadTree qt;
	Font font;
	Terrain terrain;	
};