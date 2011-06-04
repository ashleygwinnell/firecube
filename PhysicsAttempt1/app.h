class App : public Application
{
public:
    void HandleInput(float t);
    void Render(float t);
    void Update(float t);
    NodePtr model, sphere;
    ShaderPtr vShader, pShader, pShader2;
    ProgramPtr program, program2;
    FontPtr font;
};