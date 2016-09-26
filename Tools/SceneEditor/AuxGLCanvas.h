#pragma once

#include "FireCube.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>

class MyApp;
class EditorState;
class GLCanvas;

class AuxGLCanvas : public wxGLCanvas, public FireCube::Object
{
	FIRECUBE_OBJECT(AuxGLCanvas);
	friend class CanvasDropTarget;
public:
	AuxGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("AuxGLCanvas"));

	~AuxGLCanvas();

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
	FireCube::Node *GetRootNode();

	template <typename T>
	void SetConstructSceneCallback(T &&callbck)
	{
		constructSceneCallback = callbck;
	}
	
private:
	void CreateGrid(float size, unsigned int numberOfCells);
	
	bool init;
	MyApp *theApp;
	wxGLContext *context;
	FireCube::Scene *scene, *editorScene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *cameraTarget;
		
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
	
	EditorState *editorState;
	FireCube::SharedPtr<FireCube::SceneView> sceneView;
	std::function<void(AuxGLCanvas *)> constructSceneCallback;
};