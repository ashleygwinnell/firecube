#include "TextureCube.h"
#include "Utils/Filesystem.h"
#include "Utils/Image.h"
#include <array>

using namespace FireCube;

TextureCube::TextureCube(Engine *engine) : Texture(engine)
{
	target = GL_TEXTURE_CUBE_MAP;
}

bool TextureCube::Load(const std::string &filename)
{	
	std::string fileExt = Filesystem::GetFileExtension(filename);
	std::string baseFilename = Filesystem::RemoveFileExtension(filename);

	std::array<std::string, 6> cubeFacesFilePostfix = { "_posx", "_negx", "_posy", "_negy", "_posz", "_negz"};
	std::array<GLenum, 6> cubeFacesTargets = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

	glBindTexture(target, objectId);

	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string currentFaceFileName = baseFilename + cubeFacesFilePostfix[i] + "." + fileExt;
		GLenum currentFaceTarget = cubeFacesTargets[i];

		Image img(engine);
		if (!img.Load(currentFaceFileName))
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

		width = img.GetWidth();
		height = img.GetHeight();
		
		glTexImage2D(currentFaceTarget, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);				
		GenerateMipMaps();
		SetFiltering(TextureFilter::MIPMAP);
	}

	SetFiltering(TextureFilter::LINEAR);
	
	return true;
}