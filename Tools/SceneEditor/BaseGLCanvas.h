#pragma once

#include "FireCube.h"
#include <wx/glcanvas.h>

class MyApp;

class BaseGLCanvas : public wxGLCanvas, public FireCube::Object
{	
public:
	FIRECUBE_OBJECT(BaseGLCanvas);
	BaseGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("BaseGLCanvas"));

	~BaseGLCanvas();

	bool IsInitialized() const;

protected:
	MyApp *theApp;
private:

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event);

	virtual void Render() = 0;
	virtual void Init() = 0;

	void InitCanvas();	

	bool init;
	
	wxGLContext *context;	
};