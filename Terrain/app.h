class App : public Application, public InputListener
{
public:
    bool Init();
    void HandleInput(float time, const MappedInput &input);
    void Update(float time);
    void Render(float time);

    NodePtr root, node;
    ProgramPtr program;
    FontPtr font;
    Terrain terrain;
};