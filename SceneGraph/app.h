class App : public Application, public InputListener
{
public:
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    bool Prepare();
    FontFacePtr fontFace;
    NodePtr root;
	CameraPtr orthographicCamera;
	CameraPtr camera;
	Scene scene;
};