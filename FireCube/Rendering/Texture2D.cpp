#include "Texture2D.h"
#include "Utils/Filesystem.h"
#include "Utils/Image.h"

using namespace FireCube;

Texture2D::Texture2D(Engine *engine) : Texture(engine)
{
	target = GL_TEXTURE_2D;
}

bool Texture2D::Load(const std::string &filename)
{
	if (Filesystem::GetFileExtension(filename) == "dds")
	{
		return LoadDDS(filename);
	}

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

	width = img.GetWidth();
	height = img.GetHeight();

	glBindTexture(target, objectId);
	glTexImage2D(target, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, &img.GetPixels()[0]);
	GenerateMipMaps();
	SetFiltering(TextureFilter::MIPMAP);
	return true;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

bool Texture2D::LoadDDS(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	this->filename = filename;
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(resolvedFileName.c_str(), "rb");
	if (fp == NULL)
	{
		return false;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		return false;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	height = *(unsigned int*)&(header[8]);
	width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return false;
	}

	// Create one OpenGL texture	
	glGenTextures(1, &objectId);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(target, objectId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(target, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return true;
}