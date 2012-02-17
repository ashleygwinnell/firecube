class App : public Application, public InputListener
{
    bool Init();
    void Update(float time);
    void Render(float time);
    void HandleInput(float time, const MappedInput &input);

    FontPtr font;
    NodePtr root, node, node2;
	NodePtr mainRoot;
    FrameBufferPtr fbo;
    CameraPtr camera;
	CameraPtr orthographicCamera;
};