#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "GLee.h"

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
#include "Image.h"
#include "Font.h"
#include "ShaderGenerator.h"
#include "Renderer.h"				
#include "Application.h"


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
		for (int y=0;y<height;y++)
		{
			for (int x=0;x<width;x++)
			{
				for (int j=0;j<bpp/8;j++)
				{				
					unsigned int i=y*height*bpp/8+x*bpp/8+j;
					data[i]=((unsigned char*)(image->pixels))[y*image->pitch+x*bpp/8+j];
				}
			}
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