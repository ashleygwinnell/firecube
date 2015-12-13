#pragma once

#include "Texture.h"

namespace FireCube
{

class FIRECUBE_API TextureCube : public Texture
{
	FIRECUBE_OBJECT(TextureCube)
public:
	TextureCube(Engine *engine);

	/**
	* Loads a texture.
	* @param filename The file to load.
	*/
	virtual bool Load(const std::string &filename);
private:

};

}