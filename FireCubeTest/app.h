class App : public Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
	ModelManager modelManager;
	Model model;	
	Font font;
};