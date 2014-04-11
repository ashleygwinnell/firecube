class App : public Application, public InputListener
{
public:
	App();
	~App();
	void HandleInput(float t, const MappedInput &input);
	void Render(float t);
	void Update(float t);
	bool Prepare();    
	Node *root;
	NodeObserverCamera *camera;
	Camera *orthographicCamera;
	Scene scene;
	Font *font;
	FontFace *fontFace;
};