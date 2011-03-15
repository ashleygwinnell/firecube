class App : public Application
{
public:
    bool Init();
    void HandleInput(float time);
    void Update(float time);
    void Render(float time);

    Node root, node;
    Program program;
    Font font;
    Terrain terrain;
};