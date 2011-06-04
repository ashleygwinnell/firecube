#include <FireCube.h>
using namespace std;
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
    frame = new MyMainFrame(NULL);
    frame->Show(true);
    return true;
}
void FireCubeApp::HandleInput(float t)
{
}
void FireCubeApp::Render(float t)
{
}
void FireCubeApp::Update(float t)
{
}

void MyApp::LoadDocument(const string &filename)
{
    FireCube::Timer t;
    t.Init();
    if (!document.Load(filename))
		return;
    ostringstream oss3;
    oss3 << "Loading completed in " << t.Passed() << " seconds.";
    frame->SetStatusBarText(oss3.str());
    if (applicationParameters.customProgram)
        document.GetRoot()->SetProgram(applicationParameters.program);

	document.GenerateNormals(applicationParameters.normalsLength);
	document.GenerateTangents(applicationParameters.normalsLength);
	document.GenerateBitangents(applicationParameters.normalsLength);
	
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
