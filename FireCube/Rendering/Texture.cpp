#include <sstream>
#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/Texture.h"
#include "Utils/Image.h"
#include "Core/Engine.h"

using namespace FireCube;

Texture::Texture(Engine *engine) : Resource(engine), GraphicsResource(engine->GetRenderer()), minFilter(LINEAR), magFilter(LINEAR)
{
	Create();
}

Texture::~Texture()
{
	LOGINFO("Destroyed texture with id=", objectId;);
	glDeleteTextures(1, &objectId);
	objectId = 0;
}

bool Texture::Create()
{
	glGenTextures(1, &objectId);		
	LOGINFO("Created texture with id=", objectId);
	return objectId != 0;
}

bool Texture::Load(const std::string &filename)
{	
	Image img(engine);
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
	
	glBindTexture(GL_TEXTURE_2D, objectId);        		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);		
	GenerateMipMaps();
	SetFiltering(MIPMAP, LINEAR);		
	return true;
}

void Texture::GenerateMipMaps()
{
	glBindTexture(GL_TEXTURE_2D, objectId);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetFiltering(TextureFilter minFilter, TextureFilter magFilter)
{
	this->minFilter = minFilter;
	this->magFilter = magFilter;
}