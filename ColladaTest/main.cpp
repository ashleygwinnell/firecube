#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <Windows.h>
#include <gl/GL.h>
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"

App app;
int main(int argc, char *argv[])
{
	if (!app.Initialize())
		return 0;
	app.Run();
	return 0;
}
bool App::Init()
{
	SetTitle("ColladaTest");
	font=Renderer::GetFontManager()->Create("c:\\windows\\fonts\\arial.ttf",18);
	if (!font)
		return false;


	DAE colladaDom;
	daeInt result = colladaDom.load("cube.dae");

	if(result != DAE_OK) {
		std::cout << result << std::endl;
		return 0;
	}
	
	vector<domMesh*> meshes = colladaDom.getDatabase()->typeLookup<domMesh>();
	daeUInt meshCount = meshes.size();
	std::cout << "Num meshes: " << meshCount << std::endl;

	domMesh *thisMesh=meshes[0];
		
	domSource_Array &sourceArr = thisMesh->getSource_array();
	std::cout << "Num sources: " << sourceArr.getCount() << endl;
	
	return true;
}
void App::Render(float time)
{
	Renderer::Clear(vec4(0.7f,0.7f,0.7f,1.0f),1.0f);
	Renderer::SetPerspectiveProjection(90.0f,0.1f,100.0f);
	Renderer::SetOrthographicProjection();
	Renderer::SetModelViewMatrix(mat4());
	ostringstream oss;
	oss << "FPS: " << app.GetFps();
	Renderer::RenderText(font,vec2(0,0),vec4(1,1,1,1),oss.str());
}
void App::Update(float time)
{

}
void App::HandleInput(float time)
{

}