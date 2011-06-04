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
#include "GLee.h"

#include "utils.h"
#include "Logger.h"
#include "Texture.h"

using namespace FireCube;

Texture::Texture() : id(0)
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
        Create();
        this->filename = filename;

        if (image->format->BytesPerPixel == 4)
        {
            if (image->format->Rshift == 16)
                format = GL_BGRA;
            else
                format = GL_RGBA;
        }
        if (image->format->BytesPerPixel == 3)
        {
            if (image->format->Rshift == 16)
                format = GL_BGR;
            else
                format = GL_RGB;
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, image->format->BytesPerPixel, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
    GLint min, mag;

    if (minFilter == NEAREST)
        min = GL_NEAREST;
    else if (minFilter == LINEAR)
        min = GL_LINEAR;
    else if (minFilter == MIPMAP)
        min = GL_LINEAR_MIPMAP_LINEAR;

    if (minFilter == NEAREST)
        mag = GL_NEAREST;
    else if (minFilter == LINEAR)
        mag = GL_LINEAR;
    else if (minFilter == MIPMAP)
        mag = GL_LINEAR_MIPMAP_LINEAR;

    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

}
string Texture::GetFileName() const
{
    return filename;
}
unsigned int Texture::GetId() const
{
    return id;
}
