#include <FireCube.h>
using namespace FireCube;
#include <wx/wx.h>
#include "BaseGLCanvas.h"
#include "mainframe.h"
#include "MainFrameImpl.h"
#include "app.h"
#include "Types.h"
#include "Gizmos/TranslateGizmo.h"
#include "Gizmos/RotateGizmo.h"
#include "Gizmos/ScaleGizmo.h"
#include "MainFrameImpl.h"
#include "Commands/TransformCommands.h"
#include "Commands/RemoveNodeCommand.h"
#include "Commands/AddNodeCommand.h"
#include "Commands/AddComponentCommand.h"
#include "Commands/GroupCommand.h"
#include "Descriptors/PhysicsWorldDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "AssetUtils.h"
#include "SceneReader.h"

BaseGLCanvas::BaseGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, nullptr, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()),
	init(false), theApp((MyApp*)wxTheApp)
{
	context = theApp->GetMainContext(this);
	
	Bind(wxEVT_SIZE, &BaseGLCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &BaseGLCanvas::OnPaint, this);
	Bind(wxEVT_ERASE_BACKGROUND, &BaseGLCanvas::OnEraseBackground, this);	
}

BaseGLCanvas::~BaseGLCanvas()
{
	Unbind(wxEVT_SIZE, &BaseGLCanvas::OnSize, this);
	Unbind(wxEVT_PAINT, &BaseGLCanvas::OnPaint, this);
	Unbind(wxEVT_ERASE_BACKGROUND, &BaseGLCanvas::OnEraseBackground, this);	
}

bool BaseGLCanvas::IsInitialized() const
{
	return init;
}

void BaseGLCanvas::InitCanvas()
{
	if (!init)
	{
		init = true;
		theApp->InitEngine();
		Init();
		glEnable(GL_POINT_SPRITE); // Needed since wxWidgets 3.0 creates an old OpenGL context (not using wglCreateContextAttribs)
	}
}

void BaseGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	wxPaintDC dc(this);
	if (!SetCurrent(*context))
		return;
	
	InitCanvas();	

	int w, h;
	GetClientSize(&w, &h);

	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);	

	Render();

	SwapBuffers();

	
}
void BaseGLCanvas::OnSize(wxSizeEvent& event)
{
	if (!IsShownOnScreen())
		return;

	int w, h;
	GetClientSize(&w, &h);
	if (!SetCurrent(*context))
		return;
	
	InitCanvas();	

	engine->GetRenderer()->SetWidth(w);
	engine->GetRenderer()->SetHeight(h);
}

void BaseGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	// Do nothing, to avoid flashing.
}