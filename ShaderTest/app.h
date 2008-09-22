class App : public Application
{
public:
	bool HandleInput(float t);
	bool Render(float t);
	bool Update(float t);
	ModelManager modelManager;
	Font font;
	Model model;
};