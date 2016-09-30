#pragma once

#include "FireCube.h"
#include "BaseGLCanvas.h"
#include <wx/dnd.h>

class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;
class ScaleGizmo;
class EditorState;
class NodeDescriptor;
class EditorCanvas;

enum class Operation
{
	NONE, OBJECT_TRANSFORM, CAMERA_ORBIT, PICK_MATERIAL
};

class CanvasDropTarget : public wxDropTarget
{
public:
	CanvasDropTarget(EditorCanvas *canvas);
private:

	virtual wxDragResult OnData(wxCoord vX, wxCoord vY, wxDragResult eResult) override;
	virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def) override;
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def) override;

	EditorCanvas *canvas;
};

class EditorCanvas : public BaseGLCanvas
{
	FIRECUBE_OBJECT(EditorCanvas);	
	friend class CanvasDropTarget;
public:	
	EditorCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("GLCanvas"));

	~EditorCanvas();

	void OnEnterWindow(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	virtual void Render() override;
	virtual void Init() override;
	void UpdateGizmo();		
	void SetRootDescriptor(NodeDescriptor *rootDescriptor);	
private:	
	void CreateGrid(float size, unsigned int numberOfCells);
	void SelectedNodeChanged(NodeDescriptor *nodeDesc);
	void StateChanged();
	void SceneChanged();
	void RenderDebugGeometry(NodeDescriptor *nodeDesc, FireCube::DebugRenderer *debugRenderer);
	void StartMaterialPick();
	void AddMesh(const std::string &path);
	void AddPrefab(const std::string &path);
	
	FireCube::Scene *scene, *editorScene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *cameraTarget, *editorRoot;
	FireCube::SharedPtr<TranslateGizmo> translateGizmo;
	FireCube::SharedPtr<RotateGizmo> rotateGizmo;
	FireCube::SharedPtr<ScaleGizmo> scaleGizmo;
	TransformGizmo *transformGizmo;
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	Operation currentOperation;	
	EditorState *editorState;	
	NodeDescriptor *rootDesc;
};