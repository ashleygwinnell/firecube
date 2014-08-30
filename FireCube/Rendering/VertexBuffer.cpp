#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/VertexBuffer.h"

using namespace FireCube;

const unsigned int VertexBuffer::attributeSize[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)] = {
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

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, VertexAttributeType vertexAttributes, BufferType bt)
{
	this->vertexAttributes.clear();	
	for (unsigned int i = 0; i < static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes) & (1 << i))
		{
			AddVertexAttribute(static_cast<VertexAttributeType>(1 << i), attributeSize[i]);			
		}
		else
		{
			AddVertexAttribute(VertexAttributeType::NONE, 0);
		}
	}

	return LoadData(data, vertexCount, bt);
}

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, BufferType bt)
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
	for (auto &vertexAttribute : vertexAttributes)
	{
		vertexAttribute.offset = currentOffset;			
		currentOffset += vertexAttribute.size;	
	}
	vertexSize = currentOffset;
}

void VertexBuffer::ApplyAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	for (unsigned int i = 0; i < vertexAttributes.size(); ++i)
	{
		const VertexAttribute &vertexAttribute = vertexAttributes[i];
		if (vertexAttribute.size > 0)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vertexAttribute.size / sizeof(float), GL_FLOAT, GL_FALSE, vertexSize, (void *)vertexAttribute.offset);
		}
		else
		{
			glDisableVertexAttribArray(i);
		}
	}

	for (unsigned int i = vertexAttributes.size(); i < 16; ++i)
	{
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

const std::vector<VertexAttribute> &VertexBuffer::GetVertexAttributes() const
{
	return vertexAttributes;
}

void VertexBuffer::AddVertexAttribute(VertexAttributeType type, int size)
{
	vertexAttributes.push_back(VertexAttribute());
	vertexAttributes.back().type = type;
	vertexAttributes.back().size = size;
}

unsigned int VertexBuffer::GetVertexAttributeOffset(VertexAttributeType type)
{
	for (const auto &vertexAttribute : vertexAttributes)
	{
		if (vertexAttribute.type == type)
		{
			return vertexAttribute.offset;
		}
	}
	
	return 0;
}

bool VertexBuffer::HasAttribute(VertexAttributeType type)
{
	for (const auto &vertexAttribute : vertexAttributes)
	{
		if (vertexAttribute.type == type)
		{
			return true;
		}
	}

	return false;
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

unsigned int VertexBuffer::GetVertexSize(VertexAttributeType vertexAttributes)
{
	unsigned int currentOffset = 0;
	for (unsigned int i = 0; i < static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes) & (1 << i))
		{			
			currentOffset += attributeSize[i];
		}
	}
	return currentOffset;
}

unsigned int VertexBuffer::GetAttributeSize(VertexAttributeType vertexAttribute)
{
	return attributeSize[VertexBuffer::GetVertexAttributeIndex(vertexAttribute)];
}

unsigned int VertexBuffer::GetVertexAttributeIndex(VertexAttributeType vertexAttribute)
{
	switch (vertexAttribute)
	{
	case VertexAttributeType::POSITION:
		return 0;
		break;
	case VertexAttributeType::NORMAL:
		return 1;
		break;
	case VertexAttributeType::TEXCOORD0:
		return 2;
		break;
	case VertexAttributeType::TANGENT:
		return 3;
		break;
	case VertexAttributeType::COLOR:
		return 4;
		break;
	default:
		return 0;
		break;
	}
}

VertexAttributeType FireCube::operator | (const VertexAttributeType &lhs, const VertexAttributeType &rhs)
{
	VertexAttributeType ret = static_cast<VertexAttributeType>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return ret;
}

VertexAttributeType FireCube::operator |= (VertexAttributeType &lhs, const VertexAttributeType &rhs)
{
	lhs = static_cast<VertexAttributeType>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return lhs;
}

VertexAttributeType FireCube::operator & (const VertexAttributeType &lhs, const VertexAttributeType &rhs)
{
	VertexAttributeType ret = static_cast<VertexAttributeType>(static_cast<int>(lhs)& static_cast<int>(rhs));
	return ret;
}