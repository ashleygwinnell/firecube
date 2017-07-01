#pragma once

#include "Texture.h"

namespace FireCube
{

/**
* A 2d texture.
*/
class FIRECUBE_API Texture2D : public Texture
{
	FIRECUBE_OBJECT(Texture2D)
public:
	Texture2D(Engine *engine);

	/**
	* Loads a texture.
	* @param filename The file to load.
	*/
	virtual bool Load(const std::string &filename);

	void SetSize(unsigned int width, unsigned int height, TextureFormat format);
	void SetData(unsigned int width, unsigned int height, TextureFormat format, void *data);
	void SetData(unsigned int x, unsigned int y, unsigned int width, unsigned int height, TextureFormat format, void *data);
private:
	bool LoadDDS(const std::string &filename);

};

}