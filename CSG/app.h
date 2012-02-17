class App : public Application, public InputListener
{
public:
	bool Init();
	void Update(float time);
	void Render(float time);
	void HandleInput(float time, const MappedInput &input);
private:	
	CameraPtr orthographicCamera;
	FontPtr font;
	NodePtr root;
	NodeObserverCameraPtr camera;
};