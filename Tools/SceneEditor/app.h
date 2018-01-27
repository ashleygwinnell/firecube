#pragma once

#include "wx/app.h"
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
	void Render(float t);
	void Update(float t) {};
	void HandleSDLEvent(SDL_Event &event);
	virtual bool Prepare() override;
	void HandleInput(float dt, const FireCube::MappedInput &input);
	void OpenSceneFile(const std::string &filename);
	void Reset();
	void LoadSettingsFile();
	void WriteSettingsFile();
	void SavePrefabs(NodeDescriptor *node);
	void Exit();
	void CollectCameras(NodeDescriptor *node);
	void UpdateCamerasList();
	void ComponentAdded(ComponentDescriptor *componentDesc);
	void ComponentRemoved(ComponentDescriptor *componentDesc);

	FireCube::Scene *scene;	
	EditorState *editorState;
	NodeDescriptor rootDesc;	
	std::vector<std::string> recentSceneFiles;
	std::vector<std::pair<std::string, CameraDescriptor *>> cameras;

	EditorWindow *editorWindow;	
	HierarchyWindow *hierarchyWindow;
	InspectorWindow *inspectorWindow;
	AssetBrowserWindow *assetBrowserWindow;
	MaterialEditorWindow *materialEditorWindow;

	bool showFileOpen;
	bool showNewDialog;
	bool showSaveAs;
};

class MyApp : public wxApp
{
public:
	~MyApp();
	virtual bool OnInit();
	FireCube::Scene *GetScene();
	FireCube::Scene *GetEditorScene();
	EditorState *GetEditorState();
	void InitScene();
	void InitEngine();
	MainFrameImpl *GetMainFrame();
	FireCubeApp fcApp;	

	void SetMainContext(wxGLContext *mainContext);
	wxGLContext *GetMainContext(wxGLCanvas *glCanvas);
	
private:	
	MainFrameImpl *frame;
	FireCube::Scene *scene;
	FireCube::Scene *editorScene;
	EditorState *editorState;	
	wxGLContext *mainContext;
	bool engineInitialized;
};