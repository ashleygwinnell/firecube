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
		glDeleteFramebuffers(1, &id);
		id=0;
	}
	if (depthBuffer)
	{
		glDeleteRenderbuffers(1, &depthBuffer);
		depthBuffer=0;
	}
}
void FrameBuffer::Create(int width,int height)
{
	resource=boost::shared_ptr<FrameBufferResource>(new FrameBufferResource);
	resource->width=width;
	resource->height=height;
	glGenFramebuffers(1,&resource->id);	
}
void FrameBuffer::SetRenderTarget(Texture texture,int attachmentPoint)
{	
	resource->texture[attachmentPoint]=texture;
	Renderer::UseTexture(texture,0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,resource->width,resource->height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+attachmentPoint,GL_TEXTURE_2D,texture.GetId(),0);	
	glBindFramebuffer(GL_FRAMEBUFFER,resource->id);	
}
void FrameBuffer::AddDepthBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,resource->id);
	glGenRenderbuffers(1,&resource->depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,resource->depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,resource->width,resource->height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,resource->depthBuffer);
}
void FrameBuffer::AddDepthBufferTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER,resource->id);	
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
	
	glFramebufferTexture2D (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, resource->depthTexture.GetId(), 0);
}
void FrameBuffer::AddRenderTarget(int attachmentPoint)
{
	glBindFramebuffer(GL_FRAMEBUFFER,resource->id);			
	glDrawBuffer(GL_FRONT_AND_BACK);	
	resource->texture[attachmentPoint].Create();
	Renderer::UseTexture(resource->texture[attachmentPoint],0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,resource->width,resource->height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0_EXT+attachmentPoint,GL_TEXTURE_2D,resource->texture[attachmentPoint].GetId(),0);	
}
Texture FrameBuffer::GetRenderTarget(int attachmentPoint)
{
	return resource->texture[attachmentPoint];
}
Texture FrameBuffer::GetDepthBuffer()
{
	return resource->depthTexture;
}

bool FrameBuffer::IsValid()
{
	if (resource->id==0)
		return false;
		
	glBindFramebuffer(GL_FRAMEBUFFER,resource->id);
	bool found=false;
	for (int i=0;(i<MAX_TEXTURES) && (!found);i++)
		if ((resource->texture[i]) && (resource->texture[i].IsValid()))
			found=true;
	if (found)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
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
