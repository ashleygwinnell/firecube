#include <algorithm>
#include <sstream>

#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Utils/Image.h"
#include "stb_image.h"
#include "jpgd.h"

using namespace FireCube;

Image::Image() : width(0), height(0), bytesPerPixel(0)
{

}

bool Image::Load(const std::string &filename)
{
	std::string fname = Filesystem::SearchForFileName(filename);
	if (fname.empty())
		return false;

	unsigned int pos = filename.find_last_of('.');
	std::string ext;
	bool loadedUsingStb = false;
	if (pos != std::string::npos)
	{
		ext = filename.substr(pos + 1);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	}
	unsigned char *pixels;
	if (pos == std::string::npos || ext != "jpg")
	{
		loadedUsingStb = true;
		pixels = stbi_load(filename.c_str(), &width, &height, &bytesPerPixel, 0);
		if (!pixels)
		{
			std::ostringstream oss;
			oss << "Failed loading image: " << filename << " reason: " << stbi_failure_reason();
			Logger::Write(Logger::LOG_ERROR, oss.str());
			return false;
		}
	}
	else
	{		
		pixels = jpgd::decompress_jpeg_image_from_file(filename.c_str(), &width, &height, &bytesPerPixel, 3);
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