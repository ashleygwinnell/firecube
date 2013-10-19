#include "Resource.h"
using namespace FireCube;

Resource::Resource(Engine *engine) : Object(engine)
{

}

std::string Resource::GetFileName() const
{
	return filename;
}