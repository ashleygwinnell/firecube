class App : public Application
{
public:
    bool Init();
    void HandleInput(float time);
    void Update(float time);
    void Render(float time);

    NodePtr root, node;
    ProgramPtr program;
    FontPtr font;
    Terrain terrain;
};