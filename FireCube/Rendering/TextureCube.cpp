#include "TextureCube.h"
#include "Utils/Filesystem.h"
#include "Utils/Image.h"
#include <array>

using namespace FireCube;

TextureCube::TextureCube(Engine *engine) : Texture(engine)
{
	target = GL_TEXTURE_CUBE_MAP;
	wrapMode = TextureWrapMode::CLAMP_TO_EDGE;
}

bool TextureCube::Load(const std::string &filename)
{	
	std::string fileExt = Filesystem::GetFileExtension(filename);
	std::string baseFilename = Filesystem::RemoveFileExtension(filename);

	std::array<std::string, 6> cubeFacesFilePostfix = { "_posx", "_negx", "_posy", "_negy", "_posz", "_negz"};
	std::array<GLenum, 6> cubeFacesTargets = { GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z}; // pos/neg y are flipped since in OpenGL, sampling a cube map assumes the
																										 // origin of the texture to be in the upper left instead of lower left is
																										 // in all other texture sampling



	glBindTexture(target, objectId);

	for (unsigned int i = 0; i < 6; ++i)
	{
		std::string currentFaceFileName = baseFilename + cubeFacesFilePostfix[i] + "." + fileExt;
		GLenum currentFaceTarget = cubeFacesTargets[i];

		Image img(engine);
		if (!img.Load(currentFaceFileName))
			return false;

		this->filename = filename;

		if (img.GetBytesPerPixel() == 4)
		{
			format = TextureFormat::RGBA;

		}
		else if (img.GetBytesPerPixel() == 3)
		{
			format = TextureFormat::RGB;
		}
		else
		{
			return false;
		}

		width = img.GetWidth();
		height = img.GetHeight();

		if (format == TextureFormat::RGB)
		{
			glTexImage2D(currentFaceTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);
		}
		else if (format == TextureFormat::RGBA)
		{
			glTexImage2D(currentFaceTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);
		}
	}	

	GenerateMipMaps();
	SetFiltering(TextureFilter::MIPMAP);
	
	return true;
}