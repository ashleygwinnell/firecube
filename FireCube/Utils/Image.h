#ifndef IMAGE_H
#define IMAGE_H

#pragma warning(push)
#pragma warning(disable:4251)

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
    bool Load(const std::string &filename);
    /**
    * Returns the width of the image.
    */
    int GetWidth() const;
    /**
    * Returns the height of the image.
    */
    int GetHeight() const;
    /**
    * Returns the number of bits per pixel of the image.
    */
    int GetBitsPerPixel() const;
    /**
    * Returns the image pixel data.
    */
    std::vector<unsigned char> &GetPixels();
    /**
    * Returns a pixel.
    */
    vec4 GetPixel(int x, int y) const;
private:
    int width, height, bitsPerPixel;
    std::vector<unsigned char> data;
};
}

#pragma warning(pop)

#endif