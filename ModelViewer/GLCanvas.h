#ifndef GLCANVAS_H
#define GLCANVAS_H

class MyApp;
class GLCanvas : public wxGLCanvas
{

public:
	class RenderingParameters
	{
	public:
		FireCube::vec4 bgColor;
		GLenum renderingMode;
		bool cullFaceEnabled;
		bool renderNormals;
		bool renderTangents;		
		FireCube::BufferPtr gridVertex;
		DWORD gridCount;
		FireCube::ProgramPtr plainColorProgram;
		FireCube::NodeObserverCameraPtr camera;
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
    void CreateGrid(float size, DWORD numberOfCells);
    void RenderGrid();
    void Init();
	void SetBackgroundColor(FireCube::vec4 color);
	FireCube::vec4 GetBackgroundColor();
	void SetRenderingMode(GLenum renderingMode);
	void SetCullFace(bool enabled);
	bool GetCullFace();
	void SetRenderingNormals(bool enabled);
	bool GetRenderingNormals();
	void SetRenderingTangents(bool enabled);
	bool GetRenderingTangents();
	void SetRotation(FireCube::vec3 rotation);
	FireCube::vec3 GetRotation();
	void SetZoom(float v);
	float GetZoom() const;
    
private:
    bool init;
    FireCube::vec2 lastpos;
	RenderingParameters renderingParameters;
	MyApp *theApp;    
};

#endif