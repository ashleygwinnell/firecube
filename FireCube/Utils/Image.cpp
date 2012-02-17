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

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Math/MyMath.h"
#include "Utils/Image.h"

using namespace FireCube;

Image::Image() : width(0), height(0), bitsPerPixel(0)
{

}
bool Image::Load(const string &filename)
{
    SDL_Surface *image;
    string fname = Filesystem::SearchForFileName(filename);
    if (fname.empty())
        return false;
    image = IMG_Load(fname.c_str());
    if (image)
    {
        bitsPerPixel = image->format->BitsPerPixel;
        width = image->w;
        height = image->h;
        data.resize(width * height * bitsPerPixel / 8);
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int j = 0; j < bitsPerPixel / 8; j++)
                {
                    unsigned int i = y * height * bitsPerPixel / 8 + x * bitsPerPixel / 8 + j;
                    data[i] = ((unsigned char*)(image->pixels))[y * image->pitch + x * bitsPerPixel / 8 + j];
                }
            }
        }
        SDL_FreeSurface(image);

        return true;
    }
    return false;
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
    return bitsPerPixel;
}
vector<unsigned char> &Image::GetPixels()
{
    return data;
}
vec4 Image::GetPixel(int x, int y) const
{
	if (bitsPerPixel == 8)
	{
		return vec4(data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 0], 0.0f, 0.0f, 1.0f) / 255.0f;
	}
	else if (bitsPerPixel == 16)
	{
		return vec4(data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 0], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 1], 0.0f, 1.0f) / 255.0f;
	}
	else if (bitsPerPixel == 24)
	{
		return vec4(data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 0], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 1], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 2], 1.0f) / 255.0f;
	}
	else if (bitsPerPixel == 32)
	{
		return vec4(data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 0], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 1], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 2], data[y * width * bitsPerPixel / 8 + x * bitsPerPixel / 8 + 3]) / 255.0f;
	}
	return vec4();
}