#include "FireCube.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "MainFrameImpl.h"
#include "app.h"

using namespace FireCube;

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	engineInitialized = false;
	scene = nullptr;
	editorScene = nullptr;
	editorState = new EditorState(fcApp.GetEngine());	
	mainContext = nullptr;
	wxInitAllImageHandlers();
	frame = new MainFrameImpl(nullptr);
	SetTopWindow(frame);
	frame->Maximize();
	frame->Show(true);
	return true;
}

FireCube::Scene *MyApp::GetScene()
{
	return scene;
}

FireCube::Scene *MyApp::GetEditorScene()
{
	return editorScene;
}

EditorState *MyApp::GetEditorState()
{
	return editorState;
}

void MyApp::InitScene()
{
	scene = new FireCube::Scene(fcApp.GetEngine());			
	editorScene = new FireCube::Scene(fcApp.GetEngine());
}

void MyApp::InitEngine()
{
	if (!engineInitialized)
	{
		engineInitialized = true;

		Filesystem::SetCoreDataFolder("../../FireCube");

		fcApp.InitializeNoWindow();
	}
}

MyApp::~MyApp()
{
	delete scene;
	delete editorScene;
}

MainFrameImpl *MyApp::GetMainFrame()
{
	return frame;
}

void MyApp::SetMainContext(wxGLContext *mainContext)
{
	this->mainContext = mainContext;
}

wxGLContext *MyApp::GetMainContext(wxGLCanvas *glCanvas)
{		
	if (!mainContext)
	{
		mainContext = new wxGLContext(glCanvas);
	}
	return mainContext;
}
