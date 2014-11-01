#include "FireCube.h"
using namespace FireCube;
#include "Project.h"

Project::Project()
{

}

Project::~Project()
{
	delete scene;
}

void Project::Init(Engine *engine)
{
	scene = new Scene(engine);	
}

Scene *Project::GetScene()
{
	return scene;
}