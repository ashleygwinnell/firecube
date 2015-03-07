#include "ThirdParty/GLEW/glew.h"
#include "Rendering/Texture.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Renderer.h"
#include "Core/Engine.h"
#include "Rendering/RenderSurface.h"

using namespace FireCube;

FrameBuffer::FrameBuffer(Engine *engine) : Object(engine), GraphicsResource(engine->GetRenderer()), depthSurface(nullptr)
{
	for (int i = 0; i < MAX_TEXTURES; ++i)
		texture[i] = nullptr;
}

FrameBuffer::~FrameBuffer()
{	
    if (objectId)
    {
        glDeleteFramebuffers(1, &objectId);
        objectId = 0;
    }	
}

void FrameBuffer::Create(const int width, const int height)
{
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &objectId);
}

void FrameBuffer::SetRenderTarget(Texture *texture, const int attachmentPoint)
{
	if (texture)
	{
		this->texture[attachmentPoint] = texture;

		glBindFramebuffer(GL_FRAMEBUFFER, objectId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture->GetObjectId(), 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, objectId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, 0, 0);
	}
    
}

void FrameBuffer::SetDepthBufferSurface(RenderSurface *depthSurface)
{
	this->depthSurface = depthSurface;

	if (depthSurface->GetLinkedTexture())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, objectId);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthSurface->GetLinkedTexture()->GetObjectId(), 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, objectId);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthSurface->GetObjectId());
	}
}

Texture *FrameBuffer::GetRenderTarget(const int attachmentPoint)
{
    return texture[attachmentPoint];
}

RenderSurface *FrameBuffer::GetDepthSurface()
{
    return depthSurface;
}

bool FrameBuffer::IsValid() const
{
    if (objectId == 0)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, objectId);    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    return true;
}

int FrameBuffer::GetWidth() const
{
    return width;
}

int FrameBuffer::GetHeight() const
{
    return height;
}
