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
	std::ostringstream ss;
	ss << "Created buffer with id=" << objectId;
	LOGINFO(ss.str());
}

bool IndexBuffer::LoadData(void *data, unsigned int count, BufferType bt)
{
	GLenum e;
	if (bt == STREAM)
		e = GL_STREAM_DRAW;
	else if (bt == DYNAMIC)
		e = GL_DYNAMIC_DRAW;
	else if (bt == STATIC)
		e = GL_STATIC_DRAW;
	else
		e = GL_STATIC_DRAW;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, e);
	if (glGetError() == GL_OUT_OF_MEMORY)
		return false;
	return true;
}

void IndexBuffer::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectId);
}
void IndexBuffer::Destroy()
{
	std::ostringstream ss;
	ss << "Destroyed buffer with id=" << objectId;
	LOGINFO(ss.str());
	glDeleteBuffers(1, &objectId);
	objectId = 0;
}