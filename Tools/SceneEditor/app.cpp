#include "FireCube.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "app.h"
#include <imgui.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "HierarchyWindow.h"
#include "EditorWindow.h"
#include "InspectorWindow.h"
#include "AssetWindow.h"
#include "SceneReader.h"
#include "tinyxml.h"
#include "Commands/Command.h"
#include "Commands/AddNodeCommand.h"
#include "SceneWriter.h"
#include "AssetBrowserWindow.h"
#include "RenderingWindow.h"
#include "Descriptors/CameraDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "Descriptors/BoxDescriptor.h"
#include "Descriptors/SphereDescriptor.h"
#include "Descriptors/PlaneDescriptor.h"
#include "Descriptors/CollisionShapeDescriptor.h"
#include "Descriptors/CharacterControllerDescriptor.h"
#include "Descriptors/LightDescriptor.h"
#include "Descriptors/LuaScriptDescriptor.h"
#include "Descriptors/ParticleEmitterDescriptor.h"
#include "Descriptors/RigidBodyDescriptor.h"
#include "Descriptors/TerrainDescriptor.h"
#include "Descriptors/GridDescriptor.h"
#include "Commands/AddComponentCommand.h"
#include "MaterialEditorWindow.h"
#include "IconsForkAwesome.h"
#include <shlobj.h>

using namespace FireCube;

std::string BrowseFolder(const std::string &title);

#ifdef _DEBUG
int main(int argc, char *argv[])
{
	FireCubeApp app;
	Filesystem::SetCoreDataFolder("../../FireCube");

	if (!app.Initialize(WindowProperties().Maximized(true)))
		return 0;

	app.Run();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	app.WriteSettingsFile();
	if (app.currentProjectPath.empty() == false)
	{
		app.project.Save(app.editorState, Filesystem::JoinPath(app.currentProjectPath, ".project"));
	}
	return 0;
}
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	FireCubeApp app;
	Filesystem::SetCoreDataFolder("../../FireCube");

	if (!app.Initialize(WindowProperties().Maximized(true)))
		return 0;

	app.Run();
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	app.WriteSettingsFile();
	if (app.currentProjectPath.empty() == false)
	{
		app.project.Save(app.editorState, Filesystem::JoinPath(app.currentProjectPath, ".project"));
	}
	return 0;
}
#endif


FireCubeApp::FireCubeApp() : showSettingsPopup(false), showImportMeshPopup(false), 
	showImportScriptPopup(false), showImportMaterialPopup(false), showImportTexturePopup(false), showImportTechniquePopup(false)
{

}

void FireCubeApp::Render(float t)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(GetWindow());
	ImGui::NewFrame();


	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
	flags |= ImGuiWindowFlags_NoDocking;
	
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::Begin("MainWindow", nullptr, flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	RenderMenuBar();
	RenderToolbar();
	
	ImGuiID dockspaceId = ImGui::GetID("MainWindowDockspace");
	ImGui::DockSpace(dockspaceId);

	ImGui::End();
	ImGui::PopStyleVar();

	hierarchyWindow->Render();
	editorWindow->Render();
	inspectorWindow->Render();
	assetWindow->Render();
	assetBrowserWindow->Render();
	materialEditorWindow->Render();
	renderingWindow->Render();
	
	static char externalCodeEditorPath[1024];
	static char gameExecutablePath[1024];
	if (showSettingsPopup == true)
	{
		ImGui::OpenPopup("Settings");
		std::copy(settings->externalCodeEditorPath.begin(), settings->externalCodeEditorPath.end(), externalCodeEditorPath);
		std::copy(project.gameExecutablePath.begin(), project.gameExecutablePath.end(), gameExecutablePath);
		externalCodeEditorPath[settings->externalCodeEditorPath.size()] = '\0';
		gameExecutablePath[project.gameExecutablePath.size()] = '\0';
		showSettingsPopup = false;
	}

	if (ImGui::BeginPopupModal("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText("External Code Editor", externalCodeEditorPath, 1024);
		ImGui::Separator();
		ImGui::InputText("Game Executable Path", gameExecutablePath, 1024);
		ImGui::Separator();
		if (ImGui::Button("Ok"))
		{
			settings->externalCodeEditorPath = externalCodeEditorPath;
			project.gameExecutablePath = gameExecutablePath;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (showImportMeshPopup == true)
	{
		ImGui::OpenPopup("Import Mesh");
		importAssetPath[0] = '\0';
		selectedMeshMaterials.clear();
		showImportMeshPopup = false;
	}

	if (showImportScriptPopup == true)
	{
		ImGui::OpenPopup("Import Script");
		importAssetPath[0] = '\0';
		showImportScriptPopup = false;
	}

	if (showImportMaterialPopup == true)
	{
		ImGui::OpenPopup("Import Material");
		importAssetPath[0] = '\0';
		showImportMaterialPopup = false;
	}

	if (showImportTexturePopup == true)
	{
		ImGui::OpenPopup("Import Texture");
		importAssetPath[0] = '\0';
		showImportTexturePopup = false;
	}

	if (showImportTechniquePopup == true)
	{
		ImGui::OpenPopup("Import Technique");
		importAssetPath[0] = '\0';
		showImportTechniquePopup = false;
	}

	RenderImportMeshDialog();

	if (ImGui::BeginPopupModal("Import Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::InputText("##Path", importAssetPath, 1024);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = importAssetPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(importAssetPath);
			ofn.lpstrFilter = "Lua\0*.lua\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileNameA(&ofn);
		}
		ImGui::Separator();
		if (ImGui::Button("Import"))
		{
			std::string scriptFileName = importAssetPath;
			std::replace(scriptFileName.begin(), scriptFileName.end(), '/', '\\');

			if (!Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), scriptFileName))
			{
				std::string filename = Filesystem::GetLastPathComponent(scriptFileName);
				std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts";
				Filesystem::CreateFolder(targetPath);
				Filesystem::CopyPath(scriptFileName, targetPath + Filesystem::PATH_SEPARATOR + filename);
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Import Material", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::InputText("##Path", importAssetPath, 1024);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = importAssetPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(importAssetPath);
			ofn.lpstrFilter = "Material\0*.xml\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileNameA(&ofn);
		}
		ImGui::Separator();
		if (ImGui::Button("Import"))
		{
			std::string materialFileName = importAssetPath;
			std::replace(materialFileName.begin(), materialFileName.end(), '/', '\\');

			AssetUtils::ImportMaterialIfNeeded(materialFileName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Import Texture", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::InputText("##Path", importAssetPath, 1024);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = importAssetPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(importAssetPath);
			ofn.lpstrFilter = "Images\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileNameA(&ofn);
		}
		ImGui::Separator();
		if (ImGui::Button("Import"))
		{
			std::string textureFileName = importAssetPath;
			std::replace(textureFileName.begin(), textureFileName.end(), '/', '\\');

			AssetUtils::ImportTextureIfNeeded(textureFileName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Import Technique", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::InputText("##Path", importAssetPath, 1024);
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = importAssetPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(importAssetPath);
			ofn.lpstrFilter = "Technique\0*.xml\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileNameA(&ofn);
		}
		ImGui::Separator();
		if (ImGui::Button("Import"))
		{
			std::string techniqueFileName = importAssetPath;
			std::replace(techniqueFileName.begin(), techniqueFileName.end(), '/', '\\');

			AssetUtils::ImportTechniqueIfNeeded(techniqueFileName);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	for (unsigned int i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		renderer->SetRenderTarget(i, nullptr);
	}
	renderer->SetDepthSurface(nullptr);
	renderer->UpdateFrameBuffer();
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);		
	renderer->Clear(vec4(0.0f, 0.0f, 0.0f, 1.0f), 1.0, ClearBufferType::COLOR | ClearBufferType::DEPTH);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FireCubeApp::HandleSDLEvent(SDL_Event &event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

bool FireCubeApp::Prepare()
{
	editorState = new EditorState(GetEngine());
	settings = &editorState->GetSettings();

	LoadSettingsFile();

	SetTitle("SceneEditor");

	SubscribeToEvent(Events::HandleInput, &FireCubeApp::HandleInput);
	SubscribeToEvent(editorState, editorState->componentAdded, &FireCubeApp::ComponentAdded);
	SubscribeToEvent(editorState, editorState->componentRemoved, &FireCubeApp::ComponentRemoved);
	SubscribeToEvent(editorState, editorState->showMaterialEditor, &FireCubeApp::ShowMaterialEditor);
	SubscribeToEvent(editorState, editorState->openScene, &FireCubeApp::OpenSceneFile);
	SubscribeToEvent(editorState, editorState->nodeRenamed, &FireCubeApp::NodeRenamed);
	GetInputManager().AddMapping(Key::Z, InputMappingType::ACTION, "Undo", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::Y, InputMappingType::ACTION, "Redo", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::N, InputMappingType::ACTION, "New", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::O, InputMappingType::ACTION, "Open", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::S, InputMappingType::ACTION, "Save", KeyModifier::CTRL);
	GetInputManager().AddMapping(Key::S, InputMappingType::ACTION, "SaveAs", KeyModifier::CTRL | KeyModifier::SHIFT);
	GetInputManager().AddMapping(Key::X, InputMappingType::ACTION, "Exit", KeyModifier::CTRL);
	
	editorWindow = new EditorWindow(engine);
	hierarchyWindow = new HierarchyWindow(engine);
	inspectorWindow = new InspectorWindow(engine);
	assetWindow = new AssetWindow(engine);
	assetBrowserWindow = new AssetBrowserWindow(engine);
	materialEditorWindow = new MaterialEditorWindow(engine);
	renderingWindow = new RenderingWindow(engine);
	
	ImGui::CreateContext();
	const char* glslVersion = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(GetWindow(), GetGLContext());
	ImGui_ImplOpenGL3_Init(glslVersion);
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;

	static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	ImFontConfig iconsConfig; 
	iconsConfig.MergeMode = true; 
	iconsConfig.PixelSnapH = true;
	iconsConfig.GlyphOffset.y = 4.0f;
	io.Fonts->AddFontFromFileTTF("Data\\Fonts\\" FONT_ICON_FILE_NAME_FA, 24.0f, &iconsConfig, iconsRanges);

	scene = new FireCube::Scene(GetEngine());	
		
	editorWindow->SetScene(scene, &rootDesc, editorState);
	hierarchyWindow->SetScene(&rootDesc, editorState);
	inspectorWindow->SetScene(&rootDesc, editorState);
	assetWindow->SetScene(&rootDesc, editorState);
	assetBrowserWindow->SetScene(&rootDesc, editorState);
	materialEditorWindow->SetScene(&rootDesc, editorState);
	renderingWindow->SetScene(scene, &rootDesc, editorState);

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
		ShowNewDialog();
	}

	if (input.IsActionTriggered("Open"))
	{
		ShowOpenDialog();
	}

	if (input.IsActionTriggered("Save"))
	{
		Save();
	}

	if (input.IsActionTriggered("SaveAs"))
	{
		ShowSaveDialog();
	}

	if (input.IsActionTriggered("Exit"))
	{
		Exit();
	}
}

void FireCubeApp::OpenSceneFile(const std::string &filename)
{
	if (!editorState->GetCurrentSceneFile().empty())
	{
		SaveCurrentSceneFile();
	}

	editorState->SetCurrentSceneFile(filename);

	::SceneReader sceneReader(engine);

	Reset();

	if (sceneReader.Read(&rootDesc, filename))
	{
		Node *root = rootDesc.Instantiate(nullptr, engine, editorState->GetNodeMap());
		rootDesc.SetNode(root);
		scene->GetRootNode()->AddChild(root);
	}

	SetTitle("SceneEditor - " + filename);

	UpdateCamerasList();
}

void FireCubeApp::OpenProject(const std::string &path)
{
	Filesystem::SetAssetsFolder(Filesystem::JoinPath(path, "Assets"));	

	project.Load(Filesystem::JoinPath(path, ".project"));
	currentProjectPath = path;

	Reset();
	
	assetBrowserWindow->Reset();

	if (project.lastSceneFile.empty() == false)
	{
		OpenSceneFile(project.lastSceneFile);
	}

	engine->GetLuaState()->AddPackagePath(Filesystem::JoinPath(currentProjectPath, "Assets", "Scripts"));
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

	TiXmlElement *settingsElement = xmlDocument.FirstChildElement("settings");
	if (settingsElement == nullptr)
		return;

	TiXmlElement *e = settingsElement->FirstChildElement("external_code_editor");
	if (e)
	{
		settings->externalCodeEditorPath = e->Attribute("path");
	}

	e = settingsElement->FirstChildElement("recent");
	if (e)
	{
		for (TiXmlElement *element = e->FirstChildElement("file"); element != nullptr; element = element->NextSiblingElement("file"))
		{
			std::string filename = element->Attribute("name");
			if (!Filesystem::DirectoryExists(filename))
			{
				continue;
			}
			settings->recentProjectFiles.push_back(filename);
		}
	}
}

void FireCubeApp::WriteSettingsFile()
{
	TiXmlDocument doc;
	TiXmlElement *settingsElement = new TiXmlElement("settings");
	doc.LinkEndChild(settingsElement);

	TiXmlElement *element = new TiXmlElement("external_code_editor");
	settingsElement->LinkEndChild(element);
	element->SetAttribute("path", settings->externalCodeEditorPath);

	TiXmlElement *recentFilesElement = new TiXmlElement("recent");
	settingsElement->LinkEndChild(recentFilesElement);

	for (auto &projectFile : settings->recentProjectFiles)
	{
		TiXmlElement *element = new TiXmlElement("file");
		recentFilesElement->LinkEndChild(element);
		element->SetAttribute("name", projectFile);
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
	Reset();

	// Prevent destructor of NodeDescriptor from deleting the node itself since it is owned by the Scene
	rootDesc.SetNode(nullptr);
	
	Close();
}

void FireCubeApp::CollectCameras(NodeDescriptor *node)
{
	for (auto &componentDesc : node->GetComponents())
	{
		if (componentDesc->GetType() == ComponentType::CAMERA)
		{
			cameras.push_back(std::make_pair(node->GetName(), (CameraDescriptor *)componentDesc));
		}
	}

	for (auto &child : node->GetChildren())
	{
		CollectCameras(child);
	}
}

void FireCubeApp::UpdateCamerasList()
{
	cameras.clear();
	CollectCameras(&rootDesc);

	auto currentCamera = editorWindow->GetCurrentCamera();

	bool found = std::find_if(cameras.begin(), cameras.end(), [currentCamera](const std::pair<std::string, CameraDescriptor *> &camera) {
		return currentCamera == camera.second->GetComponent();
	}) != cameras.end();

	if (!found)
	{
		editorWindow->UseDefaultCamera();
	}
}

void FireCubeApp::ComponentAdded(ComponentDescriptor *componentDesc)
{	
	if (componentDesc->GetType() == ComponentType::CAMERA)
	{
		cameras.push_back(std::make_pair(componentDesc->GetParent()->GetName(), (CameraDescriptor *)componentDesc));		
	}
}

void FireCubeApp::ComponentRemoved(ComponentDescriptor *componentDesc)
{	
	if (componentDesc->GetType() == ComponentType::CAMERA)
	{
		for (unsigned int i = 0; i < cameras.size(); ++i)
		{
			if (cameras[i].second == componentDesc)
			{
				if (editorWindow->GetCurrentCamera() == componentDesc->GetComponent())
				{					
					editorWindow->UseDefaultCamera();
				}

				cameras.erase(cameras.begin() + i);
				break;
			}
		}		
	}
}

void FireCubeApp::ShowMaterialEditor()
{
	materialEditorWindow->Show();
}

void FireCubeApp::RenderMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N"))
			{
				ShowNewDialog();
			}
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				ShowOpenDialog();
			}

			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				Save();
			}

			if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
			{
				ShowSaveDialog();
			}

			if (settings->recentProjectFiles.empty() == false)
			{
				ImGui::Separator();
				std::string selectedFile;
				for (auto &filename : settings->recentProjectFiles)
				{
					if (ImGui::MenuItem(filename.c_str()))
					{
						selectedFile = filename;
					}
				}

				if (selectedFile.empty() == false && selectedFile != editorState->GetCurrentSceneFile())
				{
					settings->recentProjectFiles.erase(std::remove(settings->recentProjectFiles.begin(), settings->recentProjectFiles.end(), selectedFile), settings->recentProjectFiles.end());
					settings->recentProjectFiles.insert(settings->recentProjectFiles.begin(), selectedFile);

					OpenProject(selectedFile);
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
				auto addNodeCommand = new AddNodeCommand(editorState, "Add Node", nodeDesc, editorState->GetSelectedNode() ? editorState->GetSelectedNode() : &rootDesc);
				editorState->ExecuteCommand(addNodeCommand);
			}
			if (ImGui::BeginMenu("Component"))
			{
				if (ImGui::MenuItem("StaticModel", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto staticModelDescriptor = new StaticModelDescriptor();
					auto addComponentCommand = new AddComponentCommand(editorState, "Add StaticModel", nodeDesc, staticModelDescriptor, engine);
					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Light", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto lightDescriptor = new LightDescriptor();
					lightDescriptor->SetLightType(LightType::DIRECTIONAL);
					lightDescriptor->SetColor(vec3(1.0f));
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Light", nodeDesc, lightDescriptor, engine);
					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("LuaScript", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();

					auto luaScriptDescriptor = new LuaScriptDescriptor();
					auto addComponentCommand = new AddComponentCommand(editorState, "Add LuaScript", nodeDesc, luaScriptDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("CollisionShape", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto collisionShapeDescriptor = new CollisionShapeDescriptor();
					collisionShapeDescriptor->SetBox(BoundingBox(vec3(-0.5f), vec3(0.5f)));
					collisionShapeDescriptor->SetPlane(Plane(vec3(0.0f, 1.0f, 0.0f), 0.0f));
					collisionShapeDescriptor->SetIsTrigger(false);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add CollisionShape", nodeDesc, collisionShapeDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("CharacterController", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto characterControllerDescriptor = new CharacterControllerDescriptor();
					characterControllerDescriptor->SetRadius(0.5f);
					characterControllerDescriptor->SetHeight(1.0f);
					characterControllerDescriptor->SetContactOffset(0.1f);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add CharacterController", nodeDesc, characterControllerDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Box", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto boxDescriptor = new BoxDescriptor();
					boxDescriptor->SetSize(vec3(1.0f), engine);
					boxDescriptor->SetMaterialFileName("Materials/Default.xml", engine);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Box", nodeDesc, boxDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("RigidBody", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto rigidBodyDescriptor = new RigidBodyDescriptor();
					auto addComponentCommand = new AddComponentCommand(editorState, "Add RigidBody", nodeDesc, rigidBodyDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Plane", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto planeDescriptor = new PlaneDescriptor();
					planeDescriptor->SetSize(vec2(1.0f), engine);
					planeDescriptor->SetMaterialFileName("Materials/Default.xml", engine);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Plane", nodeDesc, planeDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Sphere", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto sphereDescriptor = new SphereDescriptor();
					sphereDescriptor->SetRadius(1.0f, engine);
					sphereDescriptor->SetColumns(16, engine);
					sphereDescriptor->SetRings(16, engine);
					sphereDescriptor->SetMaterialFileName("Materials/Default.xml", engine);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Sphere", nodeDesc, sphereDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("ParticleEmitter", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto particleEmitterDescriptor = new ParticleEmitterDescriptor();
					auto addComponentCommand = new AddComponentCommand(editorState, "Add ParticleEmitter", nodeDesc, particleEmitterDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Camera", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto cameraDescriptor = new CameraDescriptor();
					cameraDescriptor->SetOrthographic(false);
					cameraDescriptor->SetFOV(60.0f);
					cameraDescriptor->SetNearPlane(0.1f);
					cameraDescriptor->SetFarPlane(200.0f);
					cameraDescriptor->SetLeftPlane(-100.0f);
					cameraDescriptor->SetRightPlane(100.0f);
					cameraDescriptor->SetTopPlane(100.0f);
					cameraDescriptor->SetBottomPlane(-100.0f);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Camera", nodeDesc, cameraDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Terrain", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto terrainDescriptor = new TerrainDescriptor();
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Terrain", nodeDesc, terrainDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				if (ImGui::MenuItem("Grid", nullptr, nullptr, editorState->GetSelectedNode() != nullptr))
				{
					auto nodeDesc = editorState->GetSelectedNode();
					auto gridDescriptor = new GridDescriptor();
					gridDescriptor->SetSize(vec2(1.0f), engine);
					gridDescriptor->SetCountX(1, engine);
					gridDescriptor->SetCountZ(1, engine);
					gridDescriptor->SetMaterialFileName("Materials/Default.xml", engine);
					auto addComponentCommand = new AddComponentCommand(editorState, "Add Grid", nodeDesc, gridDescriptor, engine);

					editorState->ExecuteCommand(addComponentCommand);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Import"))
		{
			if (ImGui::MenuItem("Mesh"))
			{
				showImportMeshPopup = true;
			}
			if (ImGui::MenuItem("Script"))
			{
				showImportScriptPopup = true;
			}
			if (ImGui::MenuItem("Material"))
			{
				showImportMaterialPopup = true;
			}
			if (ImGui::MenuItem("Texture"))
			{
				showImportTexturePopup = true;
			}
			if (ImGui::MenuItem("Technique"))
			{
				showImportTechniquePopup = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Scene Hierarchy", nullptr, hierarchyWindow->GetIsOpenPtr());
			ImGui::MenuItem("Inspector", nullptr, inspectorWindow->GetIsOpenPtr());
			ImGui::MenuItem("Asset", nullptr, assetWindow->GetIsOpenPtr());
			ImGui::MenuItem("Material Editor", nullptr, materialEditorWindow->GetIsOpenPtr());
			ImGui::MenuItem("Asset Browser", nullptr, assetBrowserWindow->GetIsOpenPtr());
			ImGui::MenuItem("Rendering", nullptr, renderingWindow->GetIsOpenPtr());
			if (ImGui::MenuItem("Settings"))
			{
				showSettingsPopup = true;
			}
			ImGui::EndMenu();
		}

		const float CameraComboWidth = 200.0f;
		ImGui::PushItemWidth(CameraComboWidth);
		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - CameraComboWidth, 0));
		std::string selectedCameraName;
		if (editorWindow->IsUsingDefaultCamera())
		{
			selectedCameraName = "Default";
		}
		else
		{
			for (auto &camera : cameras)
			{
				if (camera.second->GetComponent() == editorWindow->GetCurrentCamera())
				{
					selectedCameraName = camera.first;
					break;
				}
			}
		}
		if (ImGui::BeginCombo("", selectedCameraName.c_str()))
		{
			if (ImGui::Selectable("Default", editorWindow->IsUsingDefaultCamera()))
			{
				editorWindow->UseDefaultCamera();
			}
			for (auto &camera : cameras)
			{
				if (ImGui::Selectable(camera.first.c_str(), editorWindow->GetCurrentCamera() == camera.second->GetComponent()))
				{
					editorWindow->UseCamera(camera.second);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::EndMenuBar();
	}
}

void FireCubeApp::RenderToolbar()
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	if (ImGui::BeginToolbar("main_tool_bar", pos, ImVec2(0, 24)))
	{
		if (ImGui::ToolbarButton(ICON_FK_ARROWS, "Translate"))
		{
			editorWindow->UseTranslateGizmo();
		}
		if (ImGui::ToolbarButton(ICON_FK_REPEAT, "Rotate"))
		{
			editorWindow->UseRotateGizmo();
		}
		if (ImGui::ToolbarButton(ICON_FK_EXPAND, "Scale"))
		{
			editorWindow->UseScaleGizmo();
		}
		if (ImGui::ToolbarButton(ICON_FK_PLAY, "Play"))
		{
			ShellExecuteA(nullptr, nullptr, Filesystem::JoinPath(currentProjectPath, project.gameExecutablePath).c_str(), nullptr, currentProjectPath.c_str(), SW_SHOW);
		}

		ImGui::EndToolbar();
	}
}

void FireCubeApp::ShowOpenDialog()
{	
	std::string path = BrowseFolder("Open Project Directory");
	if (path.empty() == false)
	{
		std::replace(path.begin(), path.end(), '/', '\\');
		OpenProject(path);

		const unsigned int maxRecentFiles = 10;
		settings->recentProjectFiles.erase(std::remove(settings->recentProjectFiles.begin(), settings->recentProjectFiles.end(), path), settings->recentProjectFiles.end());
		settings->recentProjectFiles.insert(settings->recentProjectFiles.begin(), path);
		if (settings->recentProjectFiles.size() > maxRecentFiles)
		{
			settings->recentProjectFiles.pop_back();
		}
	}
}

void FireCubeApp::ShowNewDialog()
{	
	std::string path = BrowseFolder("Choose project root directory");
	if (path.empty() == false)
	{
		std::replace(path.begin(), path.end(), '/', '\\');
		currentProjectPath = path;

		std::string assetsPath = Filesystem::JoinPath(path, "Assets");
		Filesystem::CreateFolder(assetsPath);

		Filesystem::SetAssetsFolder(assetsPath);		

		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "Scenes"));
		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "Materials"));
		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "Prefabs"));
		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "Shaders"));
		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "Techniques"));
		Filesystem::CreateFolder(Filesystem::JoinPath(assetsPath, "RenderPaths"));

		std::string targetMaterialPath = Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials", "Default.xml");
		SharedPtr<Material> defaultMaterial = new Material(engine);
		defaultMaterial->SetName("Default");
		defaultMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
		defaultMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec3(0.7f));
		defaultMaterial->SetParameter(PARAM_MATERIAL_OPACITY, 1.0f);
		defaultMaterial->SetParameter(PARAM_MATERIAL_SPECULAR, vec3(0.0f));
		defaultMaterial->SetParameter(PARAM_MATERIAL_SHININESS, 0.0f);
		AssetUtils::SerializeMaterial(defaultMaterial, targetMaterialPath);

		Reset();

		editorState->SetCurrentSceneFile("");
		engine->GetLuaState()->AddPackagePath(Filesystem::JoinPath(currentProjectPath, "Assets", "Scripts"));
	}
}

void FireCubeApp::ShowSaveDialog()
{
	char chosenPath[1024];
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = chosenPath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(chosenPath);
	ofn.lpstrFilter = "Scene\0*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	if (GetSaveFileNameA(&ofn) != 0)
	{
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
}

void FireCubeApp::RenderImportMeshDialog()
{
	if (ImGui::BeginPopupModal("Import Mesh", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Path");
		ImGui::SameLine();
		ImGui::PushItemWidth(400);
		ImGui::InputText("##Path", importAssetPath, 1024);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = importAssetPath;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(importAssetPath);
			ofn.lpstrFilter = "All\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileNameA(&ofn))
			{
				selectedMeshMaterials.clear();
				FireCube::Mesh mesh(engine);
				mesh.Load(importAssetPath);
				std::set<std::string> usedMaterials;
				for (auto &material : mesh.GetMaterials())
				{
					usedMaterials.insert(material->GetName());
				}

				for (auto &material : usedMaterials)
				{
					std::string targetMaterialPath = Filesystem::JoinPath(Filesystem::GetAssetsFolder(), "Materials", material + ".xml");
					selectedMeshMaterials.push_back(std::make_pair(material, Filesystem::FileExists(targetMaterialPath)));
				}
			}
		}
		ImGui::BeginChild("Info", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Text("Summary:");
		for (auto &material : selectedMeshMaterials)
		{
			if (material.second)
			{				
				ImGui::BulletText("Using existing material: %s", material.first.c_str());
			}
			else
			{
				ImGui::BulletText("Creating material: %s", material.first.c_str());
			}
		}
		ImGui::EndChild();
		//ImGui::Separator();
		if (ImGui::Button("Import"))
		{
			std::string meshFileName = importAssetPath;
			std::replace(meshFileName.begin(), meshFileName.end(), '/', '\\');

			if (!Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), meshFileName))
			{
				AssetUtils::ImportMesh(engine, meshFileName);
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void FireCubeApp::SaveCurrentSceneFile()
{
	if (editorState->GetCurrentSceneFile().empty())
	{
		ShowSaveDialog();
	}
	else
	{
		SceneWriter sceneWriter;
		sceneWriter.Serialize(&rootDesc, Filesystem::JoinPath(Filesystem::GetAssetsFolder(), editorState->GetCurrentSceneFile()));
		SavePrefabs(&rootDesc);
	}
}

void FireCubeApp::NodeRenamed(NodeDescriptor *node)
{
	for (auto component : node->GetComponents())
	{
		if (component->GetType() == ComponentType::CAMERA)
		{
			UpdateCamerasList();
			return;
		}
	}
}

void FireCubeApp::Save()
{
	project.Save(editorState, Filesystem::JoinPath(currentProjectPath, ".project"));
	SaveCurrentSceneFile();
}

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if (uMsg == BFFM_INITIALIZED)
	{
		//std::string tmp = (const char *)lpData;
		//std::cout << "path: " << tmp << std::endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

std::string BrowseFolder(const std::string &title)
{
	CHAR path[MAX_PATH];	

	BROWSEINFOA bi = { 0 };
	bi.lpszTitle = title.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;	

	LPITEMIDLIST pidl = SHBrowseForFolderA(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDListA(pidl, path);

		//free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return path;
	}

	return "";
}