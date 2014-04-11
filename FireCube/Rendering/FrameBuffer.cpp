#include "ThirdParty/GLEW/glew.h"
#include "Rendering/Texture.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Renderer.h"
#include "Core/Engine.h"

using namespace FireCube;

FrameBuffer::FrameBuffer(Engine *engine) : Object(engine), GraphicsResource(engine->GetRenderer()), depthBuffer(0), depthTexture(nullptr)
{
	for (int i = 0; i < MAX_TEXTURES; ++i)
		texture[i] = nullptr;
}

FrameBuffer::~FrameBuffer()
{
	delete depthTexture;
	for (int i = 0; i < MAX_TEXTURES; ++i)
		delete texture[i];

    if (objectId)
    {
        glDeleteFramebuffers(1, &objectId);
        objectId = 0;
    }
    if (depthBuffer)
    {
        glDeleteRenderbuffers(1, &depthBuffer);
        depthBuffer = 0;
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
    this->texture[attachmentPoint] = texture;
    renderer->UseTexture(texture, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture->GetObjectId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, objectId);
}

void FrameBuffer::AddDepthBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, objectId);
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::AddDepthBufferTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, objectId);
    depthTexture = new Texture(engine);
    renderer->UseTexture(depthTexture, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetObjectId(), 0);
}

void FrameBuffer::AddRenderTarget(const int attachmentPoint)
{
    glBindFramebuffer(GL_FRAMEBUFFER, objectId);
    glDrawBuffer(GL_FRONT_AND_BACK);
	texture[attachmentPoint] = new Texture(engine);
    renderer->UseTexture(texture[attachmentPoint], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture[attachmentPoint]->GetObjectId(), 0);
}

Texture *FrameBuffer::GetRenderTarget(const int attachmentPoint)
{
    return texture[attachmentPoint];
}

Texture *FrameBuffer::GetDepthBuffer()
{
    return depthTexture;
}

bool FrameBuffer::IsValid() const
{
    if (objectId == 0)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, objectId);
    bool found = false;
    for (int i = 0; (i < MAX_TEXTURES) && (!found); i++)
        if ((texture[i]) && (texture[i]->IsValid()))
            found = true;
    if (found)
        glDrawBuffer(GL_BACK);
    else
        glDrawBuffer(GL_NONE);

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
