#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Rendering/Texture.h"

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
    SDL_Surface *image;
    image = IMG_Load(filename.c_str());
    if(image)
    {
        GLenum format;
		GLenum internalFormat;
        Create();
        this->filename = filename;

        if (image->format->BytesPerPixel == 4)
        {
			internalFormat = GL_RGBA;
            if (image->format->Rshift == 16)
                format = GL_BGRA;
            else
                format = GL_RGBA;
			
        }
        if (image->format->BytesPerPixel == 3)
        {
			internalFormat = GL_RGB;
            if (image->format->Rshift == 16)
                format = GL_BGR;
            else
                format = GL_RGB;
        }

        glBindTexture(GL_TEXTURE_2D, id);        		
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);		
		GenerateMipMaps();
		SetFiltering(MIPMAP, LINEAR);		
		
        SDL_FreeSurface(image);
        return true;
    }
    return false;
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
