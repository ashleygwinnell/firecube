#include "Rendering/RenderSurface.h"
#include "Utils/Logger.h"

using namespace FireCube;

RenderSurface::RenderSurface(Renderer *renderer, RenderSurfaceType type) : GraphicsResource(renderer), type(type)
{
	glGenRenderbuffers(1, &objectId);
}

RenderSurface::~RenderSurface()
{
	Destroy();
}

void RenderSurface::Destroy()
{	
	glDeleteRenderbuffers(1, &objectId);
	objectId = 0;
}

void RenderSurface::CreateDepth(int width, int height)
{
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

RenderSurfaceType RenderSurface::GetRenderSurfaceType() const
{
	return type;
}