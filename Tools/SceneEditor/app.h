#pragma once

#include "EditorState.h"
#include "Descriptors/NodeDescriptor.h"

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

class FireCubeApp : public FireCube::Application
{
public:
	FireCubeApp();
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

	FireCube::Texture2D *rotateTexture;
	FireCube::Texture2D *translateTexture;
	FireCube::Texture2D *scaleTexture;

	bool showFileOpen;
	bool showNewDialog;
	bool showSaveAs;
	bool showSettingsPopup;
	bool showImportMeshPopup;
	bool showImportScriptPopup;
	bool showImportMaterialPopup;
private:
	void Render(float t);
	void Update(float t) {};
	void HandleSDLEvent(SDL_Event &event);
	virtual bool Prepare() override;
	void HandleInput(float dt, const FireCube::MappedInput &input);
	void OpenSceneFile(const std::string &filename);
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
	void RenderOpenDialog();
	void RenderNewDialog();
	void RenderSaveDialog();

	Settings *settings;
};