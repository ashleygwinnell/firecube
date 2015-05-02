#pragma once

#include "FireCube.h"

class MyApp;
class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;
class ScaleGizmo;
class EditorState;
class SceneDescriptor;
class NodeDescriptor;

enum class Operation
{
	NONE, OBJECT_TRANSFORM, CAMERA_ORBIT
};

class GLCanvas : public wxGLCanvas, public FireCube::Object
{
	OBJECT(GLCanvas)
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
	void OnKeyUp(wxKeyEvent& event);
	void Render();
	void Init();	
	void UpdateGizmo();	
private:	
	void CreateGrid(float size, unsigned int numberOfCells);
	void SelectedNodeChanged(NodeDescriptor *node);
	void StateChanged();
	
	bool init;
	MyApp *theApp;
	wxGLContext *context;	
	FireCube::Scene *scene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *cameraTarget;
	FireCube::SharedPtr<TranslateGizmo> translateGizmo;
	FireCube::SharedPtr<RotateGizmo> rotateGizmo;
	FireCube::SharedPtr<ScaleGizmo> scaleGizmo;
	TransformGizmo *transformGizmo;
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	Operation currentOperation;	
	EditorState *editorState;
	SceneDescriptor *sceneDescriptor;
};