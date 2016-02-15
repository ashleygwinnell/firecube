#include <sstream>
#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/Texture.h"
#include "Utils/Image.h"
#include "Core/Engine.h"
#include "Utils/Filesystem.h"

using namespace FireCube;

Texture::Texture(Engine *engine) : Resource(engine), GraphicsResource(engine->GetRenderer()), filter(TextureFilter::LINEAR), wrapMode(TextureWrapMode::REPEAT)
{
	Create();
}

Texture::~Texture()
{	
	glDeleteTextures(1, &objectId);
	objectId = 0;
}

bool Texture::Create()
{
	glGenTextures(1, &objectId);		
	return objectId != 0;
}

void Texture::GenerateMipMaps()
{
	glBindTexture(target, objectId);
	glGenerateMipmap(target);
}

void Texture::SetFiltering(TextureFilter filter)
{
	this->filter = filter;	
}

TextureFilter Texture::GetFiltering() const
{
	return filter;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

void Texture::SetWidth(int width)
{
	this->width = width;
}
void Texture::SetHeight(int height)
{
	this->height = height;
}

void Texture::SetWrapMode(TextureWrapMode wrapMode)
{
	this->wrapMode = wrapMode;
}

GLenum Texture::GetTarget() const
{
	return target;
}
