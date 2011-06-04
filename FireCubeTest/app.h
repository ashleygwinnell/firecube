class App : public Application
{
public:
    void HandleInput(float t);
    void Render(float t);
    void Update(float t);
    bool Init();
    FontPtr font;
    NodePtr root;
	CameraPtr camera;
};