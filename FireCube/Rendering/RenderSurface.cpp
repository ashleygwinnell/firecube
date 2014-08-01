#include "Rendering/RenderSurface.h"
#include "Utils/Logger.h"

using namespace FireCube;

RenderSurface::RenderSurface(Renderer *renderer) : GraphicsResource(renderer)
{

}

RenderSurface::~RenderSurface()
{
	Destroy();
}

void RenderSurface::Destroy()
{
	LOGINFO("Destroyed render surface with id=", objectId);
	glDeleteRenderbuffers(1, &objectId);
	objectId = 0;
}

void RenderSurface::CreateDepth(int width, int height)
{
	Destroy();
	glGenRenderbuffers(1, &objectId);
	glBindRenderbuffer(GL_RENDERBUFFER, objectId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	this->width = width;
	this->height = height;
}

int RenderSurface::GetWidth() const
{
	return width;
}

int RenderSurface::GetHeight() const
{
	return height;
}

void RenderSurface::SetLinkedTexture(SharedPtr<Texture> linkedTexture)
{
	this->linkedTexture = linkedTexture;
	if (linkedTexture)
	{
		width = linkedTexture->GetWidth();
		height = linkedTexture->GetHeight();
	}
}

SharedPtr<Texture> RenderSurface::GetLinkedTexture()
{
	return linkedTexture;
}