class App : public Application
{
    bool Init();
    void Update(float time);
    void Render(float time);
    void HandleInput(float time);

    FontPtr font;
    NodePtr root, node, node2;
    ProgramPtr program;
    BufferPtr uvBuffer, vBuffer;
    FrameBufferPtr fbo;
    float ang;
};