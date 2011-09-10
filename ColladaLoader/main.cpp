#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "tinyxml.h"
#include "ColladaLoader.h"

class App : public Application, public InputListener
{
public:
    virtual void Update(float t);
    virtual void HandleInput(float t, const MappedInput &input);
    virtual void Render(float t);
    NodePtr node;
};
void App::Update(float t)
{

}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
}
void App::Render(float t)
{
    Renderer::Clear(vec4(0.5f, 0.5f, 0.5f, 1), 1);
	mat4 projection;
	projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 1000.0f);
	Renderer::GetCamera()->SetProjectionMatrix(projection);    

    node->Rotate(vec3(0, t, 0));
    Renderer::Render(node);
}
void main()
{
    Filesystem::AddSearchPath("../Assets/Textures");
    App app;
	app.GetInputManager().AddInputListener(&app);
	app.GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
    ColladaLoader l("../Assets/Models/duck_triangulate.dae");
    l.Load();
    app.Initialize();
    app.node = l.GenerateSceneGraph();
    app.node->Move(vec3(0, -70, -180));

    Light light;
    light.SetAmbientColor(vec4(1, 1, 1, 1));
    light.SetDiffuseColor(vec4(1, 1, 1, 1));
	light.SetSpecularColor(vec4(0, 0, 0, 1));
    app.node->AddLight(light);
    
    app.Run();
    cout << "All OK." << endl;
}