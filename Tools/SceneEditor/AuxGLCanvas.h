#pragma once

#include "FireCube.h"
#include <wx/wx.h>
#include "BaseGLCanvas.h"

class AuxGLCanvas : public BaseGLCanvas
{
	FIRECUBE_OBJECT(AuxGLCanvas);

public:
	AuxGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("AuxGLCanvas"));

	~AuxGLCanvas();
	
	void OnEnterWindow(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	virtual void Render() override;
	virtual void Init() override;

	FireCube::Node *GetRootNode();

	template <typename T>
	void SetConstructSceneCallback(T &&callbck)
	{
		constructSceneCallback = callbck;
	}
	
private:
	void CreateGrid(float size, unsigned int numberOfCells);
		
	FireCube::Scene *scene, *editorScene;
	FireCube::OrbitCamera *camera;
	FireCube::Node *root, *gridNode, *cameraTarget;
		
	FireCube::CustomGeometry *gridGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial;
	FireCube::vec2 lastMousePos;
		
	FireCube::SharedPtr<FireCube::SceneView> sceneView;
	std::function<void(AuxGLCanvas *)> constructSceneCallback;
};