#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
using namespace std;

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Math/MyMath.h"
#include "Utils/Image.h"
#include "stb_image.h"

using namespace FireCube;

Image::Image() : width(0), height(0), bytesPerPixel(0)
{

}

bool Image::Load(const string &filename)
{
	string fname = Filesystem::SearchForFileName(filename);
	if (fname.empty())
		return false;

	unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &bytesPerPixel, 0);
	if (!pixels)
		return false;

	data.resize(width * height * bytesPerPixel);
	for (unsigned int i = 0; i < data.size(); ++i)
		data[i] = pixels[i];
	stbi_image_free(pixels);
	return true;	
	/*
	SDL_Surface *image;
	string fname = Filesystem::SearchForFileName(filename);
	if (fname.empty())
		return false;
	image = IMG_Load(fname.c_str());
	if (image)
	{		
		bytesPerPixel = image->format->BitsPerPixel / 8;
		width = image->w;
		height = image->h;
		data.resize(width * height * bytesPerPixel);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				for (int j = 0; j < bytesPerPixel; j++)
				{
					unsigned int i = y * height * bytesPerPixel + x * bytesPerPixel + j;
					data[i] = ((unsigned char*)(image->pixels))[y * image->pitch + x * bytesPerPixel + j];
				}
			}
		}
		SDL_FreeSurface(image);

		return true;
	}
	return false;*/
}

int Image::GetWidth() const
{
	return width;
}

int Image::GetHeight() const
{
	return height;
}

int Image::GetBitsPerPixel() const
{
	return bytesPerPixel * 8;
}

vector<unsigned char> &Image::GetPixels()
{
	return data;
}

vec4 Image::GetPixel(int x, int y) const
{
	if (bytesPerPixel == 1)
	{
		return vec4(data[y * width * bytesPerPixel + x * bytesPerPixel + 0], 0.0f, 0.0f, 1.0f) / 255.0f;
	}
	else if (bytesPerPixel == 2)
	{
		return vec4(data[y * width * bytesPerPixel + x * bytesPerPixel + 0], data[y * width * bytesPerPixel + x * bytesPerPixel + 1], 0.0f, 1.0f) / 255.0f;
	}
	else if (bytesPerPixel == 3)
	{
		return vec4(data[y * width * bytesPerPixel + x * bytesPerPixel + 0], data[y * width * bytesPerPixel + x * bytesPerPixel + 1], data[y * width * bytesPerPixel + x * bytesPerPixel + 2], 1.0f) / 255.0f;
	}
	else if (bytesPerPixel == 4)
	{
		return vec4(data[y * width * bytesPerPixel + x * bytesPerPixel + 0], data[y * width * bytesPerPixel + x * bytesPerPixel + 1], data[y * width * bytesPerPixel + x * bytesPerPixel + 2], data[y * width * bytesPerPixel + x * bytesPerPixel + 3]) / 255.0f;
	}
	return vec4();
}