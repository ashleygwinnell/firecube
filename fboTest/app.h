class App : public Application, public InputListener
{
    bool Prepare();
    void Update(float time);
    void Render(float time);
    void HandleInput(float time, const MappedInput &input);

    FontFacePtr fontFace;
    NodePtr root, node, node2;
	NodePtr mainRoot;
    FrameBufferPtr fbo;
    CameraPtr camera;
	CameraPtr orthographicCamera;
	Scene scene, scene2;
};