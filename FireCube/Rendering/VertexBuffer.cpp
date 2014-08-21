#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/VertexBuffer.h"

using namespace FireCube;

const unsigned int VertexBuffer::attributeSize[static_cast<int>(VertexAttribute::MAX_VERTEX_ATTRIBUTE)] = {
	3 * sizeof(float), // position
	3 * sizeof(float), // normal
	2 * sizeof(float), // texture coordinate 0
	3 * sizeof(float), // tangent	
	3 * sizeof(float)}; // color

VertexBuffer::VertexBuffer(Renderer *renderer) : GraphicsResource(renderer), isShadowed(false)
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
	LOGINFO("Created buffer with id=", objectId);
}

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, VertexAttribute vertexAttributes, BufferType bt)
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

	if (isShadowed)
	{
		shadowData.resize(vertexCount * vertexSize);
		memcpy(&shadowData[0], data, vertexCount * vertexSize);
	}

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
	for (unsigned int i = 0; i < static_cast<int>(VertexAttribute::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes) & (1 << i))
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
	for (unsigned int i = 0; i < static_cast<int>(VertexAttribute::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes)& (1 << i))
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

unsigned int VertexBuffer::GetVertexCount() const
{
	return vertexCount;
}

unsigned int VertexBuffer::GetVertexSize() const
{
	return vertexSize;
}

VertexAttribute VertexBuffer::GetVertexAttributes() const
{
	return vertexAttributes;
}

unsigned int VertexBuffer::GetVertexAttributeOffset(VertexAttribute vertexAttribute)
{
	return vertexAttributesOffset[VertexBuffer::GetVertexAttributeIndex(vertexAttribute)];
}

const std::vector<char> &VertexBuffer::GetShadowData() const
{
	return shadowData;
}

void VertexBuffer::SetShadowed(bool isShadowed)
{
	this->isShadowed = isShadowed;
}

const bool VertexBuffer::Shadowed() const
{
	return isShadowed;
}

unsigned int VertexBuffer::GetVertexSize(VertexAttribute vertexAttributes)
{
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < static_cast<int>(VertexAttribute::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes) & (1 << i))
		{			
			currentOffset += attributeSize[i];
		}
	}
	return currentOffset;
}

unsigned int VertexBuffer::GetAttributeSize(VertexAttribute vertexAttribute)
{
	return attributeSize[VertexBuffer::GetVertexAttributeIndex(vertexAttribute)];
}

unsigned int VertexBuffer::GetVertexAttributeIndex(VertexAttribute vertexAttribute)
{
	switch (vertexAttribute)
	{
	case VertexAttribute::POSITION:
		return 0;
		break;
	case VertexAttribute::NORMAL:
		return 1;
		break;
	case VertexAttribute::TEXCOORD0:
		return 2;
		break;
	case VertexAttribute::TANGENT:
		return 3;
		break;
	case VertexAttribute::COLOR:
		return 4;
		break;
	default:
		return 0;
		break;
	}
}

VertexAttribute FireCube::operator | (const VertexAttribute &lhs, const VertexAttribute &rhs)
{
	VertexAttribute ret = static_cast<VertexAttribute>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return ret;
}

VertexAttribute FireCube::operator |= (VertexAttribute &lhs, const VertexAttribute &rhs)
{
	lhs = static_cast<VertexAttribute>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return lhs;
}

VertexAttribute FireCube::operator & (const VertexAttribute &lhs, const VertexAttribute &rhs)
{
	VertexAttribute ret = static_cast<VertexAttribute>(static_cast<int>(lhs)& static_cast<int>(rhs));
	return ret;
}