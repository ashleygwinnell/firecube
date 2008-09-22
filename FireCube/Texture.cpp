#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

TextureResource::TextureResource() : id(0)
{	
}
TextureResource::~TextureResource()
{		
	ostringstream ss;
	ss<< "Destroyed texture with id="<<id<<endl;
	Logger::Write(ss.str());
	glDeleteTextures(1,&id);
	id=0;
}
bool TextureResource::IsValid()
{
	return id!=0;
}
bool TextureResource::Create()
{
	glGenTextures(1,&id);
	ostringstream ss;
	ss<< "Created texture with id="<<id<<endl;
	Logger::Write(ss.str());
	return id!=0;
}
bool TextureResource::Load(const std::string &filename)
{
	SDL_Surface *image;
	image=IMG_Load(filename.c_str());
	if(image) 
	{
		GLenum format;
		Create();

		if (image->format->BytesPerPixel==4)
			format=GL_RGBA;
		if (image->format->BytesPerPixel==3)
			format=GL_RGB;

		glBindTexture(GL_TEXTURE_2D,id);
		glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D,0,format,image->w,image->h,0,format,GL_UNSIGNED_BYTE,image->pixels);		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);			
		SDL_FreeSurface(image);		
		return true;
	}
	return false;
}
