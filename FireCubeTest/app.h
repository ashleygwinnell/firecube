class App : public Application
{
public:
	bool HandleInput(float t);
	bool Render(float t);
	bool Update(float t);
	ModelManager modelManager;
	Model model;	
	Font font;
};