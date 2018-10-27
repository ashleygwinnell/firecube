#pragma once

#include "EditorState.h"
#include "Descriptors/NodeDescriptor.h"
#include "Project.h"

class MainFrameImpl;
class wxGLContext;
class wxGLCanvas;
class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;
class ScaleGizmo;
class EditorState;
class NodeDescriptor;
class EditorCanvas;
class CameraDescriptor;
class EditorWindow;
class HierarchyWindow;
class InspectorWindow;
class AssetBrowserWindow;
class MaterialEditorWindow;
class AssetWindow;
class RenderingWindow;

class FireCubeApp : public FireCube::Application
{
public:
	FireCubeApp();
	~FireCubeApp();
	void WriteSettingsFile();

	FireCube::Scene *scene;
	EditorState *editorState;
	NodeDescriptor rootDesc;
	std::vector<std::pair<std::string, CameraDescriptor *>> cameras;

	EditorWindow *editorWindow;
	HierarchyWindow *hierarchyWindow;
	InspectorWindow *inspectorWindow;
	AssetBrowserWindow *assetBrowserWindow;
	MaterialEditorWindow *materialEditorWindow;
	AssetWindow *assetWindow;
	RenderingWindow *renderingWindow;

	CProject project;
	std::string currentProjectPath;
	
	bool showSettingsPopup;
	bool showImportMeshPopup;
	bool showImportScriptPopup;
	bool showImportMaterialPopup;
	bool showImportTexturePopup;
	bool showImportTechniquePopup;
private:
	void Render(float t);
	void Update(float t);
	void HandleSDLEvent(SDL_Event &event);
	virtual bool Prepare() override;
	void HandleInput(float dt, const FireCube::MappedInput &input);
	void OpenSceneFile(const std::string &filename);
	void OpenProject(const std::string &path);
	void Reset();
	void LoadSettingsFile();
	void SavePrefabs(NodeDescriptor *node);
	void Exit();
	void CollectCameras(NodeDescriptor *node);
	void UpdateCamerasList();
	void ComponentAdded(ComponentDescriptor *componentDesc);
	void ComponentRemoved(ComponentDescriptor *componentDesc);
	void ShowMaterialEditor();
	void RenderMenuBar();
	void RenderToolbar();
	void ShowOpenDialog();
	void ShowNewDialog();
	void ShowSaveDialog();
	void RenderImportMeshDialog();
	void SaveCurrentSceneFile();
	void NodeRenamed(NodeDescriptor *node);
	void Save();
	void InitFilesystemWatcherThread();
	void HandleFilesystemChanges();
	void FilesystemThreadFunc();

	Settings *settings;
	std::vector<std::pair<std::string, bool>> selectedMeshMaterials;
	char importAssetPath[1024];
	HANDLE assetFolderDirHandle;
	std::thread fileSystemWatcher;
	std::set<std::string> changedFiles;
	std::mutex changedFilesMutex;
};