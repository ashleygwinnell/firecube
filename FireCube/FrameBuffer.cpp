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
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Font.h"
#include "ShaderGenerator.h"
#include "RenderQueue.h"
#include "Renderer.h"
#include "Application.h"

using namespace FireCube;

FrameBufferResource::FrameBufferResource() : id(0),depthBuffer(0)
{

}
FrameBufferResource::~FrameBufferResource()
{
	if (id)
	{
		glDeleteFramebuffersEXT(1, &id);
		id=0;
	}
	if (depthBuffer)
	{
		glDeleteRenderbuffersEXT(1, &depthBuffer);
		depthBuffer=0;
	}
}
void FrameBuffer::Create(int width,int height)
{
	resource=boost::shared_ptr<FrameBufferResource>(new FrameBufferResource);
	resource->width=width;
	resource->height=height;
	glGenFramebuffersEXT(1,&resource->id);	
}
void FrameBuffer::SetRenderTarget(Texture texture,int attachmnetPoint)
{	
	resource->texture[attachmnetPoint]=texture;
	Renderer::UseTexture(texture,0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,resource->width,resource->height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0+attachmnetPoint,GL_TEXTURE_2D,texture.GetId(),0);	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,resource->id);	
}
void FrameBuffer::AddDepthBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,resource->id);
	glGenRenderbuffersEXT(1,&resource->depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,resource->depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,resource->width,resource->height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,resource->depthBuffer);
}
void FrameBuffer::AddDepthBufferTexture()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,resource->id);	
	resource->depthTexture.Create();
	Renderer::UseTexture(resource->depthTexture,0);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, resource->width, resource->height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	
	glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, resource->depthTexture.GetId(), 0);
}
void FrameBuffer::AddRenderTarget(int attachmnetPoint)
{
	glBindFramebuffer(GL_FRAMEBUFFER_EXT,resource->id);			
	glDrawBuffer(GL_FRONT_AND_BACK);	
	resource->texture[attachmnetPoint].Create();
	Renderer::UseTexture(resource->texture[attachmnetPoint],0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,resource->width,resource->height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT+attachmnetPoint,GL_TEXTURE_2D,resource->texture[attachmnetPoint].GetId(),0);	
}
Texture FrameBuffer::GetRenderTarget(int attachmnetPoint)
{
	return resource->texture[attachmnetPoint];
}
Texture FrameBuffer::GetDepthBuffer()
{
	return resource->depthTexture;
}

bool FrameBuffer::IsValid()
{
	if (resource->id==0)
		return false;
		
	glBindFramebuffer(GL_FRAMEBUFFER_EXT,resource->id);
	bool found=false;
	for (int i=0;(i<MAX_TEXTURES) && (!found);i++)
		if ((resource->texture[i]) && (resource->texture[i].IsValid()))
			found=true;
	if (found)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
		return false;
	return true;
}
int FrameBuffer::GetWidth()
{
	return resource->width;
}
int FrameBuffer::GetHeight()
{
	return resource->height;
}
unsigned int FrameBuffer::GetId() const
{
	return resource->id;
}
FrameBuffer::operator bool () const
{
	return resource;
}
bool FrameBuffer::operator== (const FrameBuffer &frameBuffer) const
{
	return resource==frameBuffer.resource;
}
