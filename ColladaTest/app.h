class App : public Application
{
public:
	bool Init();
	void Render(float time);
	void Update(float time);
	void HandleInput(float time);
	
	Font font;	
	Model model;	
};