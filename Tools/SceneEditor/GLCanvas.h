#pragma once

#include "FireCube.h"
#include <wx/dnd.h>

class MyApp;
class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;
class ScaleGizmo;
class EditorState;
class NodeDescriptor;
class GLCanvas;

enum class Operation
{
	NONE, OBJECT_TRANSFORM, CAMERA_ORBIT, PICK_MATERIAL
};

class CanvasDropTarget : public wxDropTarget
{
public:
	CanvasDropTarget(GLCanvas *canvas);
private:

	virtual wxDragResult OnData(wxCoord vX, wxCoord vY, wxDragResult eResult) override;
	virtual wxDragResult OnEnter(wxCoord x, wxCoord y, wxDragResult def) override;
	virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult def) override;

	GLCanvas *canvas;
};

class GLCanvas : public wxGLCanvas, public FireCube::Object
{
	FIRECUBE_OBJECT(GLCanvas);	
	friend class CanvasDropTarget;
public:	
	GLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("GLCanvas"));

	~GLCanvas();

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void Render();
	void Init();	
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

	bool init;
	MyApp *theApp;
	wxGLContext *context;	
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