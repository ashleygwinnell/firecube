#pragma once

#include "Texture.h"

namespace FireCube
{

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
private:
	bool LoadDDS(const std::string &filename);

};

}