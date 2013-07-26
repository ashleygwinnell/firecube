//#include <string>
//#include <vector>
//#include <fstream>
//#include <iostream>
#include <sstream>
//#include <map>
//#include <memory>
//#include <algorithm>
//using namespace std;
#include "ThirdParty/GLEW/glew.h"

//#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Rendering/Texture.h"
//#include "Math/MyMath.h"
#include "Utils/Image.h"

using namespace FireCube;

Texture::Texture() : id(0), minFilter(LINEAR), magFilter(LINEAR)
{
}

Texture::~Texture()
{
	std::ostringstream ss;
	ss << "Destroyed texture with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteTextures(1, &id);
	id = 0;
}

bool Texture::IsValid() const
{
	return id != 0;
}

bool Texture::Create()
{
	glGenTextures(1, &id);
	std::ostringstream ss;
	ss << "Created texture with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	return id != 0;
}

bool Texture::Load(const std::string &filename)
{	
	Image img;
	if (!img.Load(filename))
		return false;

	GLenum format;
	GLenum internalFormat;		
	this->filename = filename;

	if (img.GetBytesPerPixel() == 4)
	{
		internalFormat = GL_RGBA;
		format = GL_RGBA;

	}
	else if (img.GetBytesPerPixel() == 3)
	{
		internalFormat = GL_RGB;
		format = GL_RGB;
	}
	else
	{		
		return false;
	}

	Create();
	glBindTexture(GL_TEXTURE_2D, id);        		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);		
	GenerateMipMaps();
	SetFiltering(MIPMAP, LINEAR);		
	return true;
}

void Texture::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, id);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetFiltering(TextureFilter minFilter, TextureFilter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}

std::string Texture::GetFileName() const
{
	return filename;
}

unsigned int Texture::GetId() const
{
	return id;
}
