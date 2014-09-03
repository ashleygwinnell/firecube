class App : public Application, public InputListener
{
public:
	App();	
	void HandleInput(float t, const MappedInput &input);
	void Render(float t);
	void Update(float t);
	bool Prepare();    
	Node *root;
	OrbitCamera *camera;	
	Scene scene;	
	FontFace *fontFace;
};