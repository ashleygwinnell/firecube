#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include "FireCube.h"
using namespace FireCube;

Image::Image() : width(0), height(0), bpp(0)
{
	
}
bool Image::Load(const string &filename)
{
	SDL_Surface *image;
	image=IMG_Load(filename.c_str());
	if (image)
	{
		bpp=image->format->BitsPerPixel;
		width=image->w;
		height=image->h;
		data.resize(width*height*bpp/8);	
		for (int i=0;i<width*height*bpp/8;i++)
		{
			data[i]=((unsigned char*)(image->pixels))[i];
		}
		SDL_FreeSurface(image);
		return true;
	}
	return false;
}
int Image::GetWidth()
{
	return width;
}
int Image::GetHeight()
{
	return height;
}
int Image::GetBPP()
{
	return bpp;
}
vector<unsigned char> &Image::GetPixels()
{
	return data;
}
vec4 Image::GetPixel(int x,int y)
{
	if (bpp==8)
	{
		return vec4(GetPixels()[y*width*bpp/8+x*bpp/8+0],0.0f,0.0f,1.0f)/255.0f;
	}
	else if (bpp==16)
	{
		return vec4(GetPixels()[y*width*bpp/8+x*bpp/8+0],GetPixels()[y*width*bpp/8+x*bpp/8+1],0.0f,1.0f)/255.0f;
	}
	else if (bpp==24)
	{
		return vec4(GetPixels()[y*width*bpp/8+x*bpp/8+0],GetPixels()[y*width*bpp/8+x*bpp/8+1],GetPixels()[y*width*bpp/8+x*bpp/8+2],1.0f)/255.0f;
	}
	else if (bpp==32)
	{
		return vec4(GetPixels()[y*width*bpp/8+x*bpp/8+0],GetPixels()[y*width*bpp/8+x*bpp/8+1],GetPixels()[y*width*bpp/8+x*bpp/8+2],GetPixels()[y*width*bpp/8+x*bpp/8+3])/255.0f;
	}
	return vec4();
}