#include "FireCube.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "MainFrameImpl.h"
#include "app.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	scene = nullptr;
	editorScene = nullptr;
	editorState = new EditorState(fcApp.GetEngine());
	wxInitAllImageHandlers();
	frame = new MainFrameImpl(nullptr);
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

MyApp::~MyApp()
{
	delete scene;
	delete editorScene;
}

MainFrameImpl *MyApp::GetMainFrame()
{
	return frame;
}
