#include <vector>
#include <map>
#include <string>
#include <memory>
using namespace std;
#include <Windows.h>
#include "glew.h"
#include <gl/GL.h>

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Math/MyMath.h"
#include "Rendering/Texture.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

FrameBuffer::FrameBuffer() : id(0), depthBuffer(0)
{

}

FrameBuffer::~FrameBuffer()
{
    if (id)
    {
        glDeleteFramebuffers(1, &id);
        id = 0;
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
    glGenFramebuffers(1, &id);
}

void FrameBuffer::SetRenderTarget(TexturePtr texture, const int attachmentPoint)
{
    this->texture[attachmentPoint] = texture;
    Renderer::UseTexture(texture, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture->GetId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::AddDepthBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FrameBuffer::AddDepthBufferTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    depthTexture = TexturePtr(new Texture);
    depthTexture->Create();
    Renderer::UseTexture(depthTexture, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetId(), 0);
}

void FrameBuffer::AddRenderTarget(const int attachmentPoint)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glDrawBuffer(GL_FRONT_AND_BACK);
    texture[attachmentPoint] = TexturePtr(new Texture);
    texture[attachmentPoint]->Create();
    Renderer::UseTexture(texture[attachmentPoint], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture[attachmentPoint]->GetId(), 0);
}

TexturePtr FrameBuffer::GetRenderTarget(const int attachmentPoint)
{
    return texture[attachmentPoint];
}

TexturePtr FrameBuffer::GetDepthBuffer()
{
    return depthTexture;
}

bool FrameBuffer::IsValid() const
{
    if (id == 0)
        return false;

    glBindFramebuffer(GL_FRAMEBUFFER, id);
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

unsigned int FrameBuffer::GetId() const
{
    return id;
}
