#pragma once

class MyApp;
class TranslateGizmo;
class RotateGizmo;
class TransformGizmo;

enum class Operation
{
	NONE, OBJECT_TRANSFORM, CAMERA_ORBIT
};

class GLCanvas : public wxGLCanvas
{

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

private:	
	void CreateGrid(float size, unsigned int numberOfCells);
	void UpdateGizmo();
	bool init;
	MyApp *theApp;
	wxGLContext *context;
	FireCube::Engine *engine;
	FireCube::Scene *scene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *currentNode, *cameraTarget;
	FireCube::SharedPtr<TranslateGizmo> translateGizmo;
	FireCube::SharedPtr<RotateGizmo> rotateGizmo;
	TransformGizmo *transformGizmo;
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	Operation currentOperation;	
};