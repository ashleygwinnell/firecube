#include "GraphicsResource.h"
using namespace FireCube;

GraphicsResource::GraphicsResource(Renderer *renderer) : objectId(0)
{
	this->renderer = renderer;
}

GLuint GraphicsResource::GetObjectId() const
{
	return objectId;
}

bool GraphicsResource::IsValid() const
{
	return objectId != 0;
}