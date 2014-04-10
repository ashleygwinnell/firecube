class App : public Application, public InputListener
{
public:
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    void SetupLight(ProgramUniformsList &uniforms);
    void RenderShadowMap();
    void RenderDepth();
    bool Prepare();
    FrameBufferPtr fb;
    NodePtr node;
    FontFacePtr fontFace;
    ProgramPtr plain, program;
    mat4 lightProj, lightModelview;
	CameraPtr lightCamera, orthographicCamera;
	NodeObserverCameraPtr defaultCamera;    
	Scene scene, scene2;
};