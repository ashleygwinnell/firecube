class App : public Application
{
public:
    void HandleInput(float t);
    void Render(float t);
    void Update(float t);
    void SetupLight();
    void RenderShadowMap();
    void RenderDepth();
    bool Init();
    FrameBufferPtr fb;
    NodePtr node;
    FontPtr font;
    ProgramPtr plain, program;
    mat4 lightProj, lightModelview;
	CameraPtr lightCamera;
	NodeObserverCameraPtr defaultCamera;
    ProgramUniformsList programUniformsList;
    BufferPtr vBuffer;
    BufferPtr uvBuffer;
};