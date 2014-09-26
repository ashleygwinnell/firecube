#include <FireCube.h>
using namespace FireCube;
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "Document.h"
#include "GLCanvas.h"
#include "mainframe.h"
#include "mymainframe.h"
#include "main.h"

IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	applicationParameters.normalsLength = 0.1f;
	applicationParameters.customProgram = false;
	frame = new MyMainFrame(nullptr);
	frame->Show(true);
	return true;
}

void FireCubeApp::Render(float t)
{
}

void FireCubeApp::Update(float t)
{
}

void MyApp::LoadDocument(const std::string &filename)
{
	Timer t;
	t.Init();
	if (!document.Load(filename, fcApp.GetEngine()))
		return;
	std::ostringstream oss3;
	oss3 << "Loading completed in " << t.Passed() << " seconds.";
	frame->SetStatusBarText(oss3.str());
	
	document.GenerateNormals(applicationParameters.normalsLength);
	document.GenerateTangents(applicationParameters.normalsLength);
	
	frame->UpdateUI(document);
}

Document &MyApp::GetDocument()
{
	return document;
}

ApplicationParameters &MyApp::GetApplicationParameters()
{
	return applicationParameters;
}
