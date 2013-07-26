#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/Buffer.h"

using namespace FireCube;

Buffer::Buffer() : id(0)
{

}

Buffer::~Buffer()
{
	std::ostringstream ss;
	ss << "Destroyed buffer with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteBuffers(1, &id);
	id = 0;
}

void Buffer::Create()
{
	glGenBuffers(1, &id);
	std::ostringstream ss;
	ss << "Created buffer with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
}

bool Buffer::LoadIndexData(void *data, unsigned int count, BufferType bt)
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, e);
	if (glGetError() == GL_OUT_OF_MEMORY)
		return false;
	return true;
}

bool Buffer::LoadData(void *data, unsigned int size, BufferType bt)
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

	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, e);
	if (glGetError() == GL_OUT_OF_MEMORY)
		return false;
	return true;
}

void Buffer::SetIndexStream()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

bool Buffer::IsValid() const
{
	return id != 0;
}

void Buffer::Destroy()
{
	std::ostringstream ss;
	ss << "Destroyed buffer with id=" << id;
	Logger::Write(Logger::LOG_INFO, ss.str());
	glDeleteBuffers(1, &id);
	id = 0;
}

unsigned int Buffer::GetId() const
{
	return id;
}

void Buffer::SetVertexAttribute(int index, int numCoords, int stride, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, numCoords, GL_FLOAT, GL_FALSE, stride, (void*) offset);
}