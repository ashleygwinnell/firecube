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
#include "MyGLCanvas.h"
#include "mainframe.h"
#include "mymainframe.h"
#include "main.h"

IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	frame=new MyMainFrame(NULL);	
	frame->Show(true);
	return true;
}
bool FireCubeApp::HandleInput(float t)
{
	return true;
}
bool FireCubeApp::Render(float t)
{
	return true;
}
bool FireCubeApp::Update(float t)
{
	return true;
}