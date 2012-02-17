class App : public Application, public InputListener
{
public:
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    bool Init();
    FontPtr font;
    NodePtr root;
	NodeObserverCameraPtr camera;
	CameraPtr orthographicCamera;
};