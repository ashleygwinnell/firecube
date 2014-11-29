#pragma once

enum GeometryType
{
	AUX_GEOMETRY   = 0x80000000,
	GIZMO_GEOMETRY = 0x40000000,
	USER_GEOMETRY  = 0x00000001
};

class MyApp;
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
	void Render();
	void Init();	

private:	
	void CreateGrid(float size, unsigned int numberOfCells);
	bool init;
	MyApp *theApp;
	wxGLContext *context;
	FireCube::Engine *engine;
	FireCube::Scene *scene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *currentNode, *translateGizmo;	
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	std::string currentOperation;
	FireCube::vec3 dragStart;
	FireCube::vec3 startPosition;
};