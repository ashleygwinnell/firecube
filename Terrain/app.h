class App : public Application
{
public:
	bool Init();
	void HandleInput(float time);
	void Update(float time);
	void Render(float time);		
	
	Model model;	
	Program program;
	ModelManager mm;	
	Font font;
	Terrain terrain;	
};