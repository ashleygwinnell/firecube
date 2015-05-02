#include "FireCube.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "MainFrameImpl.h"
#include "app.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	editorState = new EditorState(fcApp.GetEngine());
	frame = new MainFrameImpl(nullptr);
	frame->Show(true);
	return true;
}

SceneDescriptor &MyApp::GetSceneDescriptor()
{
	return sceneDescriptor;
}

EditorState *MyApp::GetEditorState()
{
	return editorState;
}

