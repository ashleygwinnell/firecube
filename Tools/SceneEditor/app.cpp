#include "FireCube.h"
#include <wx/wx.h>
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include <wx/glcanvas.h>
#include "MainFrameImpl.h"
#include "app.h"
#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include "HierarchyWindow.h"
#include "EditorWindow.h"
#include "InspectorWindow.h"

using namespace FireCube;

/*#ifdef _DEBUG
	wxIMPLEMENT_APP_CONSOLE(MyApp);
#else
	wxIMPLEMENT_APP(MyApp);
#endif*/

bool MyApp::OnInit()
{
	engineInitialized = false;
	scene = nullptr;
	editorScene = nullptr;
	editorState = new EditorState(fcApp.GetEngine());	
	mainContext = nullptr;
	wxInitAllImageHandlers();
	frame = new MainFrameImpl(nullptr);
	SetTopWindow(frame);
	frame->Maximize();
	frame->Show(true);
	return true;
}

FireCube::Scene *MyApp::GetScene()
{
	return scene;
}

FireCube::Scene *MyApp::GetEditorScene()
{
	return editorScene;
}

EditorState *MyApp::GetEditorState()
{
	return editorState;
}

void MyApp::InitScene()
{
	scene = new FireCube::Scene(fcApp.GetEngine());			
	editorScene = new FireCube::Scene(fcApp.GetEngine());
}

void MyApp::InitEngine()
{
	if (!engineInitialized)
	{
		engineInitialized = true;

		Filesystem::SetCoreDataFolder("../../FireCube");

		fcApp.InitializeNoWindow();
	}
}

MyApp::~MyApp()
{
	delete scene;
	delete editorScene;
}

MainFrameImpl *MyApp::GetMainFrame()
{
	return frame;
}

void MyApp::SetMainContext(wxGLContext *mainContext)
{
	this->mainContext = mainContext;
}

wxGLContext *MyApp::GetMainContext(wxGLCanvas *glCanvas)
{		
	if (!mainContext)
	{
		mainContext = new wxGLContext(glCanvas);
	}
	return mainContext;
}

int main(int argc, char *argv[])
{
	FireCubeApp app;
	Filesystem::SetCoreDataFolder("../../FireCube");	

	if (!app.Initialize(WindowProperties().Maximized(true)))
		return 0;
		
	app.Run();
	return 0;
}

FireCubeApp::FireCubeApp()
{

}

void FireCubeApp::Render(float t)
{
	ImGui_ImplSdlGL3_NewFrame(GetWindow());

	ImGui::StyleColorsDark();
	
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	const ImGuiWindowFlags flags = (ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | 
									ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
	const float oldWindowRounding = ImGui::GetStyle().WindowRounding; ImGui::GetStyle().WindowRounding = 0;	
	ImGui::Begin("MainWindow", nullptr, ImVec2(0, 0), 1.0f, flags);	
	ImGui::GetStyle().WindowRounding = oldWindowRounding;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				Close();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Save Layout"))
			{
				ImGui::SaveDock("default.ini");
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::BeginDockspace();	
	{
		hierarchyWindow->Render();
		editorWindow->Render();
		inspectorWindow->Render();
	}
	ImGui::EndDockspace();
	ImGui::End();		

	for (unsigned int i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		renderer->SetRenderTarget(i, nullptr);
	}
	renderer->SetDepthSurface(nullptr);
	renderer->UpdateFrameBuffer();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);		
	renderer->Clear(vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0, ClearBufferType::COLOR | ClearBufferType::DEPTH);
	ImGui::Render();
}

void FireCubeApp::HandleSDLEvent(SDL_Event &event)
{
	ImGui_ImplSdlGL3_ProcessEvent(&event);
}

bool FireCubeApp::Prepare()
{
	SubscribeToEvent(Events::HandleInput, &FireCubeApp::HandleInput);
	GetInputManager().AddMapping(Key::Z, InputMappingType::ACTION, "Undo", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::Y, InputMappingType::ACTION, "Redo", KeyModifier::CTRL);
	editorState = new EditorState(GetEngine());
	editorWindow = new EditorWindow(engine);
	hierarchyWindow = new HierarchyWindow(engine);
	inspectorWindow = new InspectorWindow(engine);
	
	ImGui_ImplSdlGL3_Init(GetWindow());
	scene = new FireCube::Scene(GetEngine());	
		
	editorWindow->SetScene(scene, &rootDesc, editorState);
	hierarchyWindow->SetScene(&rootDesc, editorState);
	inspectorWindow->SetScene(&rootDesc, editorState);

	ImGui::LoadDock("default.ini");

	return true;
}

void FireCubeApp::HandleInput(float dt, const MappedInput &input)
{
	if (input.IsActionTriggered("Undo"))
	{
		editorState->Undo();
	}

	if (input.IsActionTriggered("Redo"))
	{
		editorState->Redo();
	}
}
