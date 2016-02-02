#include <sstream>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/Logger.h"
#include "Rendering/VertexBuffer.h"

using namespace FireCube;

const unsigned int VertexBuffer::attributeSize[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)] = {
	3,					   // position
	3,					   // normal
	2,					   // texture coordinate 0
	3,					   // tangent	
	4,					   // color
	NUM_BONES_PER_VEREX,   // blend weights
	NUM_BONES_PER_VEREX }; // blend indices

const VertexAttributeDataType VertexBuffer::attributeDataType[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)] = {
	VertexAttributeDataType::FLOAT,			  // position
	VertexAttributeDataType::FLOAT,			  // normal
	VertexAttributeDataType::FLOAT,			  // texture coordinate 0
	VertexAttributeDataType::FLOAT,			  // tangent	
	VertexAttributeDataType::FLOAT,			  // color
	VertexAttributeDataType::FLOAT,			  // blend weights
	VertexAttributeDataType::UNSIGNED_BYTE }; // blend indices

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
}

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, VertexAttributeType vertexAttributes, BufferType bt)
{
	this->vertexAttributes.clear();	
	for (unsigned int i = 0; i < static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE); ++i)
	{
		if (static_cast<int>(vertexAttributes) & (1 << i))
		{
			AddVertexAttribute(static_cast<VertexAttributeType>(1 << i), attributeDataType[i], attributeSize[i]);
		}
		else
		{
			AddVertexAttribute(VertexAttributeType::NONE, VertexAttributeDataType::FLOAT, 0);
		}
	}

	return LoadData(data, vertexCount, bt);
}

bool VertexBuffer::LoadData(void *data, unsigned int vertexCount, BufferType bt)
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
	glDeleteBuffers(1, &objectId);
	objectId = 0;
}

void VertexBuffer::UpdateAttributesOffsets()
{
	unsigned int currentOffset = 0;
	for (auto &vertexAttribute : vertexAttributes)
	{
		vertexAttribute.offset = currentOffset;			
		unsigned int size;
		switch (vertexAttribute.dataType)
		{
		case VertexAttributeDataType::FLOAT:
			size = sizeof(float);
			break;
		case VertexAttributeDataType::UNSIGNED_BYTE:
			size = sizeof(unsigned char);
			break;
		default:
			break;
		}
		currentOffset += vertexAttribute.count * size;	
	}
	vertexSize = currentOffset;
}

void VertexBuffer::ApplyAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	for (unsigned int i = 0; i < vertexAttributes.size(); ++i)
	{
		const VertexAttribute &vertexAttribute = vertexAttributes[i];
		if (vertexAttribute.count > 0)
		{
			glEnableVertexAttribArray(i);
			GLenum type;
			switch (vertexAttribute.dataType)
			{
			case VertexAttributeDataType::FLOAT:
				type = GL_FLOAT;
				break;
			case VertexAttributeDataType::UNSIGNED_BYTE:
				type = GL_UNSIGNED_BYTE;
				break;
			default:
				break;
			}

			if (vertexAttribute.dataType == VertexAttributeDataType::UNSIGNED_BYTE)
			{
				glVertexAttribIPointer(i, vertexAttribute.count, type, vertexSize, (void *)vertexAttribute.offset);
			}
			else
			{
				glVertexAttribPointer(i, vertexAttribute.count, type, GL_FALSE, vertexSize, (void *)vertexAttribute.offset);
			}
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
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void VertexBuffer::Unlock()
{
	glBindBuffer(GL_ARRAY_BUFFER, objectId);
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

void VertexBuffer::AddVertexAttribute(VertexAttributeType type, VertexAttributeDataType dataType, int count)
{
	vertexAttributes.push_back(VertexAttribute());
	vertexAttributes.back().type = type;
	vertexAttributes.back().dataType = dataType;
	vertexAttributes.back().count = count;
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
			unsigned int size;
			switch (attributeDataType[i])
			{
			case VertexAttributeDataType::FLOAT:
				size = sizeof(float);
				break;
			case VertexAttributeDataType::UNSIGNED_BYTE:
				size = sizeof(unsigned char);
				break;
			default:
				break;
			}
			currentOffset += attributeSize[i] * size;
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
	case VertexAttributeType::BONE_WEIGHTS:
		return 5;
		break;
	case VertexAttributeType::BONE_INDICES:
		return 6;
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