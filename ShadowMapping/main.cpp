#include <sstream>

#include <FireCube.h>

using namespace FireCube;
#include "app.h"
App app;

#include <cmath>
vec3 lastPos;
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;
	app.Run();
	app.Destroy();
	return 0;
}
bool App::Prepare()
{
	Filesystem::AddSearchPath("../Assets/Textures");
	SetTitle(std::string("Shadow Mapping Test Application"));
	GetInputManager().AddInputListener(this);
	GetInputManager().AddMapping(KEY_ESCAPE, ACTION, "Close");
	node = LoadMesh("../Assets/Models/scene.3ds");
	node->SetLighting(false);
	font = Renderer::GetFontPool().Create("c:\\windows\\fonts\\arial.ttf", 18);
	plain = ProgramPtr(new Program);
	plain->Create(Renderer::GetShaderPool().Create("plain.vert"), Renderer::GetShaderPool().Create("plain.frag"));
	TechniquePtr t(new Technique);
	t->LoadShader("shadowMap.vert");
	t->LoadShader("shadowMap.frag");
	Renderer::AddTechnique("shadowMap", t);
	programUniformsList.SetIntValue("shadowMap", 1);
	program = ProgramPtr(new Program);
	program->Create(Renderer::GetShaderPool().Create("1.vert"), Renderer::GetShaderPool().Create("1.frag"));	

	fb = FrameBufferPtr(new FrameBuffer(engine));
	fb->Create(1024, 1024);
	fb->AddDepthBufferTexture();	

	if (!fb->IsValid())
		return false;

	defaultCamera = NodeObserverCameraPtr(new NodeObserverCamera(GetInputManager()));
	defaultCamera->SetTarget(node);
	defaultCamera->SetMinAngX((float) (-PI / 2.0)+ 0.01f);
	defaultCamera->SetMaxAngX((float) (PI / 2.0) - 0.01f);
	defaultCamera->SetDistance(3.0f);
	lightCamera = CameraPtr(new Camera);
	orthographicCamera = CameraPtr(new Camera);
	return true;
}
void App::Update(float t)
{
}
void App::SetupLight(ProgramUniformsList &uniforms)
{
	static float t = 0.0f;
	vec3 lightPos = vec3(cos(t) * 5, 4, sin(t) * 5);
	lightProj.GeneratePerspective(45, (float)fb->GetWidth() / (float)fb->GetHeight(), 1.0f, 20.0f);
	lightModelview.LookAt(lightPos, vec3(0, 0, 0), vec3(0, 1, 0));
	lightCamera->SetProjectionMatrix(lightProj);
	lightCamera->SetPosition(lightPos);
	lightCamera->LookAt(vec3(0, 0, 0), vec3(0, 1, 0));
	uniforms.SetVec3Value("lightPos", lightPos);
	t += 0.01f;

	mat4 m = mat4::identity;
	m.Translate(0.5f, 0.5f, 0.5f);
	m.Scale(0.5f, 0.5f, 0.5f);
	m *= lightProj;
	m *= lightModelview;
	uniforms.SetMat4Value("lightMatrix", m);
}
void App::RenderShadowMap()
{		
	SetupLight(programUniformsList);
	glCullFace(GL_FRONT);
	Renderer::UseFrameBuffer(fb);
	node->SetProgram(plain);	
	Renderer::Clear(vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	Renderer::UseCamera(lightCamera);
	Renderer::Render(app.node);
	glCullFace(GL_BACK);	
}
void App::Render(float t)
{
	mat4 projection;
	projection.GeneratePerspective(90.0f, (float) GetWidth() / (float) GetHeight(), 0.1f, 100.0f);
	defaultCamera->SetProjectionMatrix(projection);
	projection.GenerateOrthographic(0, (float) app.GetWidth(), (float) app.GetHeight(), 0, 0, 1);
	orthographicCamera->SetProjectionMatrix(projection);

	RenderShadowMap();
	Renderer::RestoreFrameBuffer();
	Renderer::SetViewport(0, 0, app.GetWidth(), app.GetHeight());
	Renderer::Clear(vec4(0.2f, 0.2f, 0.6f, 1.0f), 1.0f);	
	Renderer::UseTexture(fb->GetDepthBuffer(), 1);	
	Renderer::UseCamera(defaultCamera);
	Renderer::Render(app.node, "shadowMap", programUniformsList);

	Renderer::UseCamera(orthographicCamera);
	//RenderDepth();
	std::ostringstream oss;
	oss << "FPS:" << app.GetFps();
	Renderer::RenderText(app.font, vec3(0, (float) app.GetHeight() - 20.0f, 0.0f), vec4(1, 1, 1, 1), oss.str());
}
void App::HandleInput(float t, const MappedInput &input)
{
	if (input.IsActionTriggered("Close"))
		Close();	
}
