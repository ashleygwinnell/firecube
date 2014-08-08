#ifndef GLCANVAS_H
#define GLCANVAS_H

class MyApp;
class GLCanvas : public wxGLCanvas
{

public:
	class RenderingParameters
	{
	public:
		FireCube::vec3 bgColor;
		GLenum renderingMode;
		bool cullFaceEnabled;
		bool renderNormals;
		bool renderTangents;		
		
		DWORD gridCount;		
		FireCube::NodeObserverCamera *camera;		
	};
	GLCanvas( wxWindow *parent, wxWindowID id = wxID_ANY,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = 0, const wxString& name = _T("GLCanvas") );   

	~GLCanvas();

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
	void Render();
	void Init();
	void SetBackgroundColor(FireCube::vec3 color);
	FireCube::vec3 GetBackgroundColor();
	void SetRenderingMode(GLenum renderingMode);
	void SetCullFace(bool enabled);
	bool GetCullFace();
	void SetRenderingNormals(bool enabled);
	bool GetRenderingNormals();
	void SetRenderingTangents(bool enabled);
	bool GetRenderingTangents();
	void SetRotation(FireCube::vec3 rotation);
	FireCube::vec3 GetRotation();
	void SetDistance(float v);
	float GetDistance() const;
	
private:
	bool init;
	FireCube::vec2 lastpos;
	RenderingParameters renderingParameters;
	MyApp *theApp;
	wxGLContext *context;
	FireCube::Engine *engine;
	FireCube::Scene *scene;		
};

#endif