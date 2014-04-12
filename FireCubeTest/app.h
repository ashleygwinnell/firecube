class App : public Application, public InputListener
{
public:
	App();	
	void HandleInput(float t, const MappedInput &input);
	void Render(float t);
	void Update(float t);
	bool Prepare();    
	Node *root;
	NodeObserverCamera *camera;	
	Scene scene;
	Font *font;
	FontFace *fontFace;
};