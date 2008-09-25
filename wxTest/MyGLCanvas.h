#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H
class MyGLCanvas : public wxGLCanvas
{
	
public:
	MyGLCanvas( wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("MyGLCanvas") );

	MyGLCanvas( wxWindow *parent, const MyGLCanvas *other,
		wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize, long style = 0,
		const wxString& name = _T("MyGLCanvas") );

	~MyGLCanvas();

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);
	void OnEnterWindow(wxMouseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnMouseWheel(wxMouseEvent& event);	
	void Render();
	void LoadModel(const string &filename);
	void Init();
	FireCube::Model model;
	FireCube::Font font;
	FireCube::Shader vshader,fshader;
	FireCube::Program program;
	FireCube::ModelManager mm;
	FireCube::vec4 bgColor;
	GLenum renderingMode;
	bool cullFaceEnabled;
	FireCube::Buffer normalRenderingBuffer;
	int normalRenderingBufferSize;
	FireCube::Program normalRenderingProgram;
	bool renderNormals;
private:		
	bool init;
	FireCube::vec3 rot;
	FireCube::vec2 lastpos;
	DECLARE_EVENT_TABLE()
};

#endif