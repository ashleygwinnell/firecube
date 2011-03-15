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
    FrameBuffer fb;
    Node node;
    Font font;
    Program plain, program;
    mat4 lightProj, lightModelview;
    ProgramUniformsList programUniformsList;
    Buffer vBuffer;
    Buffer uvBuffer;
};