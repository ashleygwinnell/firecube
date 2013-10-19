class App : public Application, public InputListener
{
public:
	void HandleInput(float t, const MappedInput &input);
	void Render(float t);
	void Update(float t);
	bool Prepare();    
	NodePtr root;
	NodeObserverCameraPtr camera;
	CameraPtr orthographicCamera;
	Scene scene;
	FontPtr font;
	FontFacePtr fontFace;
};