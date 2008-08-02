#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

TextureResource::TextureResource() : id(0)
{	
}
TextureResource::~TextureResource()
{		
	glDeleteTextures(1,&id);
	id=0;
}
bool TextureResource::IsValid()
{
	return id!=0;
}
bool TextureResource::Load(const std::string &filename)
{
	SDL_Surface *image;
	image=IMG_Load(filename.c_str());
	if(image) 
	{
		GLenum format;
		glGenTextures(1,&id);

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
