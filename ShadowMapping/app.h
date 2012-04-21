class App : public Application, public InputListener
{
public:
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    void SetupLight(ProgramUniformsList &uniforms);
    void RenderShadowMap();
    void RenderDepth();
    bool Init();
    FrameBufferPtr fb;
    NodePtr node;
    FontPtr font;
    ProgramPtr plain, program;
    mat4 lightProj, lightModelview;
	CameraPtr lightCamera, orthographicCamera;
	NodeObserverCameraPtr defaultCamera;
    ProgramUniformsList programUniformsList;
	GeometryPtr quad;    
	BufferPtr vBuffer;
	BufferPtr uvBuffer;
};