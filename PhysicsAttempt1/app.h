class App : public Application, public InputListener
{
public:
    void HandleInput(float t, const MappedInput &input);
    void Render(float t);
    void Update(float t);
    NodePtr model, sphere;
    ShaderPtr vShader, pShader, pShader2;
    ProgramPtr program, program2;
    FontPtr font;
};