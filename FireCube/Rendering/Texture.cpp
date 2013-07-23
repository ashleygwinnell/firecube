#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
using namespace std;
#include "glew.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Rendering/Texture.h"
#include "stb_image.h"

using namespace FireCube;

Texture::Texture() : id(0), minFilter(LINEAR), magFilter(LINEAR)
{
}

Texture::~Texture()
{
	ostringstream ss;
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
	ostringstream ss;
	ss << "Created texture with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	return id != 0;
}

bool Texture::Load(const std::string &filename)
{
	
	int width, height, channels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (!data)
		return false;
	GLenum format;
	GLenum internalFormat;		
	this->filename = filename;

	if (channels == 4)
	{
		internalFormat = GL_RGBA;
		format = GL_RGBA;

	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB;
		format = GL_RGB;
	}
	else
	{
		stbi_image_free(data);
		return false;
	}

	Create();
	glBindTexture(GL_TEXTURE_2D, id);        		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);		
	GenerateMipMaps();
	SetFiltering(MIPMAP, LINEAR);		

	stbi_image_free(data);
	return true;
	/*SDL_Surface *image;
	image = IMG_Load(filename.c_str());
	if(image)
	{
		GLenum format;
		GLenum internalFormat;		
		this->filename = filename;

		if (image->format->BytesPerPixel == 4)
		{
			internalFormat = GL_RGBA;
			if (image->format->Rshift == 16)
				format = GL_BGRA;
			else
				format = GL_RGBA;
			
		}
		else if (image->format->BytesPerPixel == 3)
		{
			internalFormat = GL_RGB;
			if (image->format->Rshift == 16)
				format = GL_BGR;
			else
				format = GL_RGB;
		}
		else
		{
			SDL_FreeSurface(image);
			return false;
		}

		Create();
		glBindTexture(GL_TEXTURE_2D, id);        		
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);		
		GenerateMipMaps();
		SetFiltering(MIPMAP, LINEAR);		
		
		SDL_FreeSurface(image);
		return true;
	}
	return false;*/
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

string Texture::GetFileName() const
{
	return filename;
}

unsigned int Texture::GetId() const
{
	return id;
}
