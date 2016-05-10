#pragma once

#include "Texture.h"

namespace FireCube
{

/**
* A cube map texture
*/
class FIRECUBE_API TextureCube : public Texture
{
	FIRECUBE_OBJECT(TextureCube)
public:
	TextureCube(Engine *engine);

	/**
	* Loads the texture.
	* @param filename The file to load. Tthe function loads 6 images by appending the following postfixes to the filename: _posx, _negx, _posy, _negy, _posz, _negz
	*/
	virtual bool Load(const std::string &filename);
private:

};

}