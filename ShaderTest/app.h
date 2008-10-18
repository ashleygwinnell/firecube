class App : public Application
{
public:
	void HandleInput(float t);
	void Render(float t);
	void Update(float t);
	bool Init();
	ModelManager modelManager;
	Font font;
	Model model;
};