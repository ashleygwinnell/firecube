#include <vector>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Windows.h>
#include "GLee.h"
#include <gl/GL.h>

#include "utils.h"
#include "Logger.h"
#include "MyMath.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Renderer.h"

using namespace FireCube;

FrameBufferResource::FrameBufferResource() : id(0), depthBuffer(0)
{

}
FrameBufferResource::~FrameBufferResource()
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
void FrameBufferResource::Create(int width, int height)
{
    this->width = width;
    this->height = height;
    glGenFramebuffers(1, &id);
}
void FrameBufferResource::SetRenderTarget(Texture texture, int attachmentPoint)
{
    this->texture[attachmentPoint] = texture;
    Renderer::UseTexture(texture, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPoint, GL_TEXTURE_2D, texture->GetId(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}
void FrameBufferResource::AddDepthBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}
void FrameBufferResource::AddDepthBufferTexture()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    depthTexture = Texture(new TextureResource);
    depthTexture->Create();
    Renderer::UseTexture(depthTexture, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetId(), 0);
}
void FrameBufferResource::AddRenderTarget(int attachmentPoint)
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glDrawBuffer(GL_FRONT_AND_BACK);
    texture[attachmentPoint] = Texture(new TextureResource);
    texture[attachmentPoint]->Create();
    Renderer::UseTexture(texture[attachmentPoint], 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT + attachmentPoint, GL_TEXTURE_2D, texture[attachmentPoint]->GetId(), 0);
}
Texture FrameBufferResource::GetRenderTarget(int attachmentPoint)
{
    return texture[attachmentPoint];
}
Texture FrameBufferResource::GetDepthBuffer()
{
    return depthTexture;
}

bool FrameBufferResource::IsValid()
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
int FrameBufferResource::GetWidth()
{
    return width;
}
int FrameBufferResource::GetHeight()
{
    return height;
}
unsigned int FrameBufferResource::GetId() const
{
    return id;
}
