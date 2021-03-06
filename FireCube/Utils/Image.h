#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <vector>

#include "Math/Math.h"
#include "Core/Resource.h"

namespace FireCube
{
/**
* A class representing an image.
*/
class FIRECUBE_API Image : public Resource
{
	FIRECUBE_OBJECT(Image)
public:
	Image(Engine *engine);
	
	void Create(int width, int height, int bytesPerPixel);

	/**
	* Loads an image.
	* @param filename The file to load.
	*/
	bool Load(const std::string &filename);
	
	bool Save(const std::string &filename);
	/**
	* Returns the width of the image.
	*/
	int GetWidth() const;
	
	/**
	* Returns the height of the image.
	*/
	int GetHeight() const;
	
	/**
	* Returns the number of bytes per pixel of the image.
	*/
	int GetBytesPerPixel() const;
	
	/**
	* Returns the image pixel data.
	*/
	std::vector<unsigned char> &GetPixels();
	
	/**
	* Returns a pixel.
	*/
	vec4 GetPixel(int x, int y) const;
private:
	int width, height, bytesPerPixel;
	std::vector<unsigned char> data;
};

}

#pragma warning(pop)
