#include <sstream>

#include <FireCube.h>
using namespace FireCube;
#include "app.h"
App app;
int main(int argc, char *argv[])
{	
	if (!app.Initialize())
		return 0;
	app.Run();
	return 0;
}
void SetNormalMap(NodePtr node, TexturePtr texture)
{	
	if (node->GetType() == Node::GEOMETRY)
	{
		GeometryPtr geometry = std::dynamic_pointer_cast<GeometryNode>(node)->GetGeometry();
		if (geometry)
			geometry->GetMaterial()->SetNormalTexture(texture);	
	}

	for (unsigned int i = 0; i < node->GetChildren().size(); i++)
		SetNormalMap(node->GetChildren()[i], texture);
}
bool App::Init()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	SetTitle(std::string("FireCube Test Application"));
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	GetInputManager().AddMapping(MOUSE_AXIS_X_RELATIVE, "mouseX");
	GetInputManager().AddMapping(MOUSE_AXIS_Y_RELATIVE, "mouseY");
	GetInputManager().AddMapping(KEY_MOUSE_LEFT_BUTTON, STATE, "RotateObject", MODIFIER_SHIFT);
	GetInputManager().AddMapping(KEY_MOUSE_RIGHT_BUTTON, STATE, "RotateLight", MODIFIER_SHIFT);

	TechniquePtr t(new Technique);
	t->LoadShader("../Assets/Shaders/debugNormals.vert");
	t->LoadShader("../Assets/Shaders/debugNormals.frag");
	Renderer::AddTechnique("debugNormals", t);
	font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);
	root = NodePtr(new Node("Root"));
	NodePtr node;
	node = LoadMesh("../Assets/Models/teapot2.3ds");    		
	root->AddChild(node);
	LightNodePtr lightNode = LightNodePtr(new LightNode("LightNode"));    
	lightNode->GetLight().SetType(DIRECTIONAL);
	lightNode->GetLight().SetAmbientColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
	lightNode->GetLight().SetDiffuseColor(vec4(0.7f, 0.7f, 0.7f, 1.0f));
	lightNode->GetLight().SetSpecularColor(vec4(0.2f, 0.2f, 0.2f, 1.0f));
	root->AddChild(lightNode);    
	SetNormalMap(root, Renderer::GetTexturePool().Create("normalMap.jpg"));
	
	camera = NodeObserverCameraPtr(new NodeObserverCamera(GetInputManager()));
	camera->SetTarget(node);
	camera->SetDistance(5.0f);
		
	orthographicCamera = CameraPtr(new Camera);
	return true;
}
void App::Update(float t)
{
	
}
void App::Render(float t)
{    
	Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);
	mat4 projection;
	projection.GeneratePerspective(60.0f, (float)GetWidth() / (float)GetHeight(), 0.1f, 100.0f);	
	camera->SetProjectionMatrix(projection);		
	Renderer::UseCamera(camera);
	Renderer::Render(root);
	mat4 ortho;
	ortho.GenerateOrthographic(0, (float) app.GetWidth(), (float) app.GetHeight(), 0, 0, 1);	
	orthographicCamera->SetProjectionMatrix(ortho);	
	Renderer::UseCamera(orthographicCamera);
	std::ostringstream oss;
	oss << "FPS:" << app.GetFps();
	Renderer::RenderText(app.font, vec3(0, (float)app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();
	
	if (input.IsStateOn("RotateObject"))
	{
		vec2 rot(-input.GetValue("mouseY"), -input.GetValue("mouseX"));
		root->GetChildren()[0]->Rotate(vec3(rot.x, rot.y, 0)*t);	
	}
	
	if (input.IsStateOn("RotateLight"))
	{
		vec2 rot(-input.GetValue("mouseY"), -input.GetValue("mouseX"));
		root->GetChildren()[1]->Rotate(vec3(rot.x, rot.y, 0)*t);		
	}
}
