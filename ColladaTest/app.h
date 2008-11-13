class App : public Application
{
public:
	bool Init();
	void Render(float time);
	void Update(float time);
	void HandleInput(float time);
	bool LoadCollada(const string &filename);
	Font font;	
	Model model;
};