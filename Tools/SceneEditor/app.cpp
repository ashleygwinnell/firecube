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
#include "imguifilesystem.h"
#include "HierarchyWindow.h"
#include "EditorWindow.h"
#include "InspectorWindow.h"
#include "SceneReader.h"
#include "tinyxml.h"
#include "Commands/Command.h"
#include "Commands/AddNodeCommand.h"
#include "SceneWriter.h"
#include "AssetBrowserWindow.h"

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

FireCubeApp::FireCubeApp() : showFileOpen(false), showNewDialog(false), showSaveAs(false)
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
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				showNewDialog = true;
			}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				showFileOpen = true;
			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				if (editorState->GetCurrentSceneFile().empty())
				{
					showSaveAs = true;
				}
				else
				{
					SceneWriter sceneWriter;
					sceneWriter.Serialize(&rootDesc, editorState->GetCurrentSceneFile());
					SavePrefabs(&rootDesc);
				}
			}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
			{
				showSaveAs = true;
			}

			if (recentSceneFiles.empty() == false)
			{
				ImGui::Separator();
				std::string selectedFile;
				for (auto &filename : recentSceneFiles)
				{
					if (ImGui::MenuItem(filename.c_str()))
					{
						selectedFile = filename;
					}
				}

				if (selectedFile.empty() == false && selectedFile != editorState->GetCurrentSceneFile())
				{									
					recentSceneFiles.erase(std::remove(recentSceneFiles.begin(), recentSceneFiles.end(), selectedFile), recentSceneFiles.end());
					recentSceneFiles.insert(recentSceneFiles.begin(), selectedFile);

					OpenSceneFile(selectedFile);
				}
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Ctrl+X"))
			{
				Exit();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (editorState->HasUndo())
			{
				std::string label = "Undo \"" + editorState->GetCurrentUndoCommand()->GetDescription() + "\"";
				if (ImGui::MenuItem(label.c_str(), "Ctrl+Z"))
				{
					editorState->Undo();
				}
			}
			else
			{
				ImGui::MenuItem("Undo", "Ctrl+Z", false, false);
			}

			if (editorState->HasRedo())
			{
				std::string label = "Redo \"" + editorState->GetCurrentRedoCommand()->GetDescription() + "\"";
				if (ImGui::MenuItem(label.c_str(), "Ctrl+Y"))
				{
					editorState->Redo();
				}
			}
			else
			{
				ImGui::MenuItem("Redo", "Ctrl+Y", false, false);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Add"))
		{
			if (ImGui::MenuItem("Node"))
			{
				auto nodeDesc = new NodeDescriptor("Node");
				auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, &rootDesc);
				editorState->ExecuteCommand(addNodeCommand);
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
		assetBrowserWindow->Render();
	}
	
	{
		static ImGuiFs::Dialog openDialog;
		const char* chosenPath = openDialog.chooseFileDialog(showFileOpen, nullptr, ".xml", "Open Scene file", ImVec2(600, 400), ImVec2(100, 100));
		std::string path = chosenPath;
		if (path.empty() == false)
		{
			std::replace(path.begin(), path.end(), '/', '\\');
			OpenSceneFile(path);
		}
	}

	{
		static ImGuiFs::Dialog newDialog;
		const char* chosenPath = newDialog.chooseFolderDialog(showNewDialog, nullptr, "Choose root assets directory", ImVec2(600, 400), ImVec2(100, 100));
		std::string path = chosenPath;
		if (path.empty() == false)
		{
			std::replace(path.begin(), path.end(), '/', '\\');

			Filesystem::SetAssetsFolder(path);

			Filesystem::CreateFolder(Filesystem::RemoveLastSeparator(path) + Filesystem::PATH_SEPARATOR + "Scenes");
			Filesystem::CreateFolder(Filesystem::RemoveLastSeparator(path) + Filesystem::PATH_SEPARATOR + "Materials");
			Filesystem::CreateFolder(Filesystem::RemoveLastSeparator(path) + Filesystem::PATH_SEPARATOR + "Prefabs");

			//SetAllPanelsVisibility(true);

			Reset();

			//assetBrowserPanel->PopulateDirectoryTree();
			//assetBrowserPanel->SetAssetsPath(Filesystem::GetAssetsFolder());

			editorState->SetCurrentSceneFile("");
		}
	}

	{
		static ImGuiFs::Dialog dialog;
		const char* chosenPath = dialog.saveFileDialog(showSaveAs, nullptr, nullptr, ".xml", "Save Scene file", ImVec2(600, 400), ImVec2(100, 100));
		std::string path = chosenPath;
		if (path.empty() == false)
		{
			SceneWriter sceneWriter;
			sceneWriter.Serialize(&rootDesc, path);
			SavePrefabs(&rootDesc);
			editorState->SetCurrentSceneFile(path);
			SetTitle("SceneEditor - " + path);
		}
	}

	ImGui::EndDockspace();
	ImGui::End();

	showFileOpen = false;
	showNewDialog = false;
	showSaveAs = false;

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
	LoadSettingsFile();

	SetTitle("SceneEditor");

	SubscribeToEvent(Events::HandleInput, &FireCubeApp::HandleInput);
	GetInputManager().AddMapping(Key::Z, InputMappingType::ACTION, "Undo", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::Y, InputMappingType::ACTION, "Redo", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::N, InputMappingType::ACTION, "New", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::O, InputMappingType::ACTION, "Open", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::S, InputMappingType::ACTION, "Save", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::S, InputMappingType::ACTION, "SaveAs", KeyModifier::CTRL | KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::X, InputMappingType::ACTION, "Exit", KeyModifier::CTRL);
	editorState = new EditorState(GetEngine());
	editorWindow = new EditorWindow(engine);
	hierarchyWindow = new HierarchyWindow(engine);
	inspectorWindow = new InspectorWindow(engine);
	assetBrowserWindow = new AssetBrowserWindow(engine);
	
	ImGui_ImplSdlGL3_Init(GetWindow());
	scene = new FireCube::Scene(GetEngine());	
		
	editorWindow->SetScene(scene, &rootDesc, editorState);
	hierarchyWindow->SetScene(&rootDesc, editorState);
	inspectorWindow->SetScene(&rootDesc, editorState);
	assetBrowserWindow->SetScene(&rootDesc, editorState);

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

	if (input.IsActionTriggered("New"))
	{
		showNewDialog = true;
	}

	if (input.IsActionTriggered("Open"))
	{
		showFileOpen = true;
	}

	if (input.IsActionTriggered("Save"))
	{
		if (editorState->GetCurrentSceneFile().empty())
		{
			showSaveAs = true;
		}
		else
		{
			SceneWriter sceneWriter;
			sceneWriter.Serialize(&rootDesc, editorState->GetCurrentSceneFile());
			SavePrefabs(&rootDesc);
		}
	}

	if (input.IsActionTriggered("SaveAs"))
	{
		showSaveAs = true;
	}

	if (input.IsActionTriggered("Exit"))
	{
		Exit();
	}
}

void FireCubeApp::OpenSceneFile(const std::string &filename)
{	
	editorState->SetCurrentSceneFile(filename);

	Filesystem::SetAssetsFolder(Filesystem::GetDirectoryName(Filesystem::GetDirectoryName(filename)));

	::SceneReader sceneReader(engine);

	Reset();

	if (sceneReader.Read(&rootDesc, filename))
	{
		Node *root = rootDesc.Instantiate(nullptr, engine, editorState->GetNodeMap());
		rootDesc.SetNode(root);
		scene->GetRootNode()->AddChild(root);
	}

	SetTitle("SceneEditor - " + filename);

	//assetBrowserPanel->PopulateDirectoryTree();
	//assetBrowserPanel->SetAssetsPath(Filesystem::GetAssetsFolder());

	//UpdateCamerasList();

	editorState->sceneChanged(editorState);
}

void FireCubeApp::Reset()
{
	editorState->ClearCommands();
	editorState->GetNodeMap().clear();

	if (scene)
	{
		scene->GetRootNode()->RemoveAllComponents();
		rootDesc.RemoveAllComponents();
		rootDesc.RemoveAllChildren();
	}

	editorState->SetSelectedNode(nullptr);
}

void FireCubeApp::LoadSettingsFile()
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile("settings.xml"))
		return;

	TiXmlElement *settings = xmlDocument.FirstChildElement("settings");
	if (settings == nullptr)
		return;

	TiXmlElement *e = settings->FirstChildElement("recent");
	if (e)
	{
		for (TiXmlElement *element = e->FirstChildElement("file"); element != nullptr; element = element->NextSiblingElement("file"))
		{
			std::string filename = element->Attribute("name");
			if (!wxFileExists(filename))
			{
				continue;
			}
			recentSceneFiles.push_back(filename);
		}
	}
}

void FireCubeApp::WriteSettingsFile()
{
	TiXmlDocument doc;
	TiXmlElement *settingsElement = new TiXmlElement("settings");
	doc.LinkEndChild(settingsElement);

	TiXmlElement *recentFilesElement = new TiXmlElement("recent");
	settingsElement->LinkEndChild(recentFilesElement);

	for (auto &sceneFile : recentSceneFiles)
	{
		TiXmlElement *element = new TiXmlElement("file");
		recentFilesElement->LinkEndChild(element);
		element->SetAttribute("name", sceneFile);
	}

	doc.SaveFile("settings.xml");
}

void FireCubeApp::SavePrefabs(NodeDescriptor *node)
{
	if (node->IsPrefab())
	{
		std::string tragetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + node->GetPrefabPath();
		SceneWriter sceneWriter;
		sceneWriter.SerializePrefab(node, tragetPath);
	}
	else
	{
		for (auto child : node->GetChildren())
		{
			SavePrefabs(child);
		}
	}
}

void FireCubeApp::Exit()
{
	/*if (editorCanvas->IsInitialized())
	{
		editorCanvas->UseDefaultCamera(); // Use default camera to prevent rendering from a soon to be deleted camera
	}*/
	Reset();

	// Prevent destructor of NodeDescriptor from deleting the node itself since it is owned by the Scene
	rootDesc.SetNode(nullptr);

	WriteSettingsFile();
	Close();
}
