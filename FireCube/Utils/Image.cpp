#include <algorithm>
#include <sstream>

#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Utils/Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "jpgd.h"

using namespace FireCube;

Image::Image(Engine *engine) : Resource(engine), width(0), height(0), bytesPerPixel(0)
{

}

void Image::Create(int width, int height, int bytesPerPixel)
{
	this->width = width;
	this->height = height;
	this->bytesPerPixel = bytesPerPixel;

	data.resize(width * height * bytesPerPixel);
}

bool Image::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;
	
	unsigned int pos = resolvedFileName.find_last_of('.');
	std::string ext;
	bool loadedUsingStb = false;
	if (pos != std::string::npos)
	{
		ext = resolvedFileName.substr(pos + 1);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	}
	unsigned char *pixels;
	if (pos == std::string::npos || ext != "jpg")
	{
		loadedUsingStb = true;
		pixels = stbi_load(resolvedFileName.c_str(), &width, &height, &bytesPerPixel, 0);
		if (!pixels)
		{			
			LOGERROR("Failed loading image: ", filename, " reason: ", stbi_failure_reason());
			return false;
		}
	}
	else
	{		
		pixels = jpgd::decompress_jpeg_image_from_file(resolvedFileName.c_str(), &width, &height, &bytesPerPixel, 3);
		if (!pixels)
			return false;
	}
	
	data.resize(width * height * bytesPerPixel);
	for (unsigned int i = 0; i < data.size(); ++i)
		data[i] = pixels[i];
	if (loadedUsingStb)
		stbi_image_free(pixels);
	else
		free(pixels);

	return true;		
}

bool Image::Save(const std::string &filename)
{
	unsigned int pos = filename.find_last_of('.');
	std::string ext;	
	if (pos != std::string::npos)
	{
		ext = filename.substr(pos + 1);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	}

	if (ext == "png")
	{
		return stbi_write_png(filename.c_str(), width, height, bytesPerPixel, data.data(), width * bytesPerPixel) != 0;
	}
	else if (ext == "bmp")
	{
		return stbi_write_bmp(filename.c_str(), width, height, bytesPerPixel, data.data()) != 0;
	}
	else if (ext == "tga")
	{
		return stbi_write_tga(filename.c_str(), width, height, bytesPerPixel, data.data()) != 0;
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

int Image::GetBytesPerPixel() const
{
	return bytesPerPixel;
}

std::vector<unsigned char> &Image::GetPixels()
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