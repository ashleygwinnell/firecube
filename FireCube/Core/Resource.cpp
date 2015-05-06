#include "Resource.h"
using namespace FireCube;

Resource::Resource(Engine *engine) : Object(engine)
{

}

void Resource::SetFileName(const std::string &filename)
{
	this->filename = filename;
}

std::string Resource::GetFileName() const
{
	return filename;
}