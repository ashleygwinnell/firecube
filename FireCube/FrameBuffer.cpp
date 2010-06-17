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
#include "Mesh.h"	
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
		glDeleteFramebuffersEXT(1, &id);
		id=0;
	}
	if (depthBuffer)
	{
		glDeleteRenderbuffersEXT(1, &depthBuffer);
		depthBuffer=0;
	}
}
void FrameBufferResource::Create(int width,int height)
{
	this->width=width;
	this->height=height;
	glGenFramebuffersEXT(1,&id);	
}
void FrameBufferResource::SetRenderTarget(Texture texture,int attachmnetPoint)
{	
	this->texture[attachmnetPoint]=texture;
	Renderer::UseTexture(texture,0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0+attachmnetPoint,GL_TEXTURE_2D,texture->id,0);	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,id);	
}
void FrameBufferResource::AddDepthBuffer()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,id);
	glGenRenderbuffersEXT(1,&depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT,width,height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthBuffer);
}
void FrameBufferResource::AddDepthBufferTexture()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,id);
	depthTexture=Texture(new TextureResource);
	depthTexture->Create();
	Renderer::UseTexture(depthTexture,0);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	
	glFramebufferTexture2DEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthTexture->id, 0);
}
void FrameBufferResource::AddRenderTarget(int attachmnetPoint)
{
	glBindFramebuffer(GL_FRAMEBUFFER_EXT,id);			
	glDrawBuffer(GL_FRONT_AND_BACK);
	texture[attachmnetPoint]=Texture(new TextureResource);
	texture[attachmnetPoint]->Create();
	Renderer::UseTexture(texture[attachmnetPoint],0);	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT+attachmnetPoint,GL_TEXTURE_2D,texture[attachmnetPoint]->id,0);	
}
Texture FrameBufferResource::GetRenderTarget(int attachmnetPoint)
{
	return texture[attachmnetPoint];
}
Texture FrameBufferResource::GetDepthBuffer()
{
	return depthTexture;
}

bool FrameBufferResource::IsValid()
{
	if (id==0)
		return false;
		
	glBindFramebuffer(GL_FRAMEBUFFER_EXT,id);
	bool found=false;
	for (int i=0;(i<MAX_TEXTURES) && (!found);i++)
		if ((texture[i]) && (texture[i]->IsValid()))
			found=true;
	if (found)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_NONE);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)!=GL_FRAMEBUFFER_COMPLETE_EXT)
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