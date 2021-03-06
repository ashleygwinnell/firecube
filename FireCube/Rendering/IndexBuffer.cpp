#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/IndexBuffer.h"

using namespace FireCube;

IndexBuffer::IndexBuffer(Renderer *renderer) : GraphicsResource(renderer)
{
	Create();
}

IndexBuffer::~IndexBuffer()
{
	Destroy();
}

void IndexBuffer::Create()
{
	glGenBuffers(1, &objectId);		
}

bool IndexBuffer::LoadData(void *data, unsigned int count, BufferType bt)
{
	GLenum e;
	switch (bt)
	{
	case BufferType::STATIC:
		e = GL_STATIC_DRAW;
		break;
	case BufferType::DYNAMIC:
		e = GL_DYNAMIC_DRAW;
		break;
	case BufferType::STREAM:
		e = GL_STREAM_DRAW;
		break;
	default:
		e = GL_STATIC_DRAW;
		break;
	}	
	
	if (isShadowed)
	{
		shadowData.resize(count * sizeof(unsigned int));
		memcpy(&shadowData[0], data, count * sizeof(unsigned int));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, e);
	if (glGetError() == GL_OUT_OF_MEMORY)
		return false;
	indicesCount = count;
	return true;
}

void IndexBuffer::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId);
}
void IndexBuffer::Destroy()
{		
	glDeleteBuffers(1, &objectId);
	objectId = 0;
}

const std::vector<char> &IndexBuffer::GetShadowData() const
{
	return shadowData;
}

void IndexBuffer::SetShadowed(bool isShadowed)
{
	this->isShadowed = isShadowed;
}

const bool IndexBuffer::Shadowed() const
{
	return isShadowed;
}

unsigned int IndexBuffer::GetIndicesCount() const
{
	return indicesCount;
}