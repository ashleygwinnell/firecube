#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/VertexBuffer.h"

using namespace FireCube;

const unsigned int VertexBuffer::attributeSize[MAX_VERTEX_ATTRIBUTE] = {
	3 * sizeof(float), // position
	3 * sizeof(float), // normal
	2 * sizeof(float), // texture coordinate 0
	3 * sizeof(float)}; // tangent	

VertexBuffer::VertexBuffer(Renderer *renderer) : GraphicsResource(renderer)
{
	Create();
}

VertexBuffer::~VertexBuffer()
{
	Destroy();
}

void VertexBuffer::Create()
{
	glGenBuffers(1, &objectId);
	std::ostringstream ss;
	ss << "Created buffer with id=" << objectId;
	LOGINFO(ss.str());
}

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, unsigned int vertexAttributes, BufferType bt)
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

	this->vertexAttributes = vertexAttributes;
	this->vertexCount = vertexCount;
	UpdateAttributesOffsets();

	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize, data, e);
	if (glGetError() == GL_OUT_OF_MEMORY)
		return false;
	return true;
}

void VertexBuffer::Destroy()
{
	std::ostringstream ss;
	ss << "Destroyed buffer with id=" << objectId;
	LOGINFO(ss.str());
	glDeleteBuffers(1, &objectId);
	objectId = 0;
}

void VertexBuffer::SetVertexAttribute(int index, int numCoords, int stride, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, numCoords, GL_FLOAT, GL_FALSE, stride, (void*) offset);
}

void VertexBuffer::UpdateAttributesOffsets()
{
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < MAX_VERTEX_ATTRIBUTE; ++i)
	{
		if (vertexAttributes & (1 << i))
		{
			vertexAttributesOffset[i] = currentOffset;
			currentOffset += attributeSize[i];
		}
	}
	vertexSize = currentOffset;
}

void VertexBuffer::ApplyAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	for (unsigned int i = 0; i < MAX_VERTEX_ATTRIBUTE; ++i)
	{
		if (vertexAttributes & (1 << i))
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, attributeSize[i] / sizeof(float), GL_FLOAT, GL_FALSE, vertexSize, (void *) vertexAttributesOffset[i]);
		}
		else
			glDisableVertexAttribArray(i);
	}
}

void *VertexBuffer::Lock()
{
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void VertexBuffer::Unlock()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}