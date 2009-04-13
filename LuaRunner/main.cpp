#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "mainframe.h"
#include "mymainframe.h"
#include "main.h"

IMPLEMENT_APP(MyApp)
using namespace FireCube;
bool MyApp::OnInit()
{	
	frame=new MyMainFrame(NULL);	
	frame->Show(true);
	return true;
}
int MyApp::OnExit()
{	
	return wxApp::OnExit();
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
