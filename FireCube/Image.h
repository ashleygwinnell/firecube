#ifndef IMAGE_H
#define IMAGE_H

namespace FireCube
{
/**
* A class representing an image.
*/
class FIRECUBE_API Image
{
public:
	Image();
	/**
	* Loads an image.
	* @param filename The file to load.
	*/
	bool Load(const string &filename);
	/**
	* Returns the width of the image.
	*/
	int GetWidth();
	/**
	* Returns the height of the image.
	*/
	int GetHeight();
	/**
	* Returns the number of bits per pixel of the image.
	*/
	int GetBPP();
	/**
	* Returns the image pixel data.
	*/
	vector<unsigned char> &GetPixels();
	/**
	* Returns a pixel.
	*/
	vec4 GetPixel(int x,int y);
private:
	int width,height,bpp;
	vector<unsigned char> data;
};
}
#endif