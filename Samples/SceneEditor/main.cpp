#include "FireCube.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "MainFrameImpl.h"
#include "app.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	frame = new MainFrameImpl(nullptr);
	frame->Show(true);
	return true;
}

Project &MyApp::GetProject()
{
	return project;
}

