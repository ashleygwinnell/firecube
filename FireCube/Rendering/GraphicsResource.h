#pragma once

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"

namespace FireCube
{

class Renderer;

class FIRECUBE_API GraphicsResource
{
public:
	GraphicsResource(Renderer *renderer);
	GLuint GetObjectId() const;

	/**
	* Returns whether the buffer is valid.
	*/
	bool IsValid() const;

protected:
	Renderer *renderer;
	GLuint objectId;
};

}