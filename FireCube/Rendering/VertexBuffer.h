#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Rendering/GraphicsResource.h"

namespace FireCube
{
/**
* Specifies how data is handled.<br>
* Buffer type:<br>
* STATIC:  Data is sent once and stored on the graphics card.<br>
* DYNAMIC: Graphics driver will choose the location for the data.<br>
* STREAM:  Data is sent each time.
*/
enum class BufferType
{
	STATIC, DYNAMIC, STREAM
};

#define NUM_BONES_PER_VEREX 4

enum class FIRECUBE_API VertexAttributeType
{
	NONE = 0, POSITION = 1, NORMAL = 2, TEXCOORD0 = 4, TANGENT = 8, COLOR = 16, BONE_WEIGHTS = 32, BONE_INDICES = 64, CUSTOM = 128, MAX_VERTEX_ATTRIBUTE = 7
};

enum class FIRECUBE_API VertexAttributeDataType
{
	FLOAT, UNSIGNED_BYTE
};

VertexAttributeType operator | (const VertexAttributeType &lhs, const VertexAttributeType &rhs);
VertexAttributeType operator |= (VertexAttributeType &lhs, const VertexAttributeType &rhs);
VertexAttributeType operator & (const VertexAttributeType &lhs, const VertexAttributeType &rhs);

class VertexAttribute
{
public:
	VertexAttributeType type;
	VertexAttributeDataType dataType;
	unsigned int count;
	unsigned int offset;	
};

/**
* A buffer of data used for rendering.
*/
class FIRECUBE_API VertexBuffer : public GraphicsResource
{
public:
	VertexBuffer(Renderer *renderer);
	~VertexBuffer();

	/**
	* Creates a new buffer.
	*/
	void Create();
	
	/**
	* Loads the specified data to the buffer.
	* @param data Pointer to the data to be stored.	
	* @param vertexCount The number of vertices
	* @param bt The buffer type.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int vertexCount, BufferType bt);

	/**
	* Loads the specified data to the buffer.
	* @param data Pointer to the data to be stored.
	* @param vertexCount Size of the data in bytes.
	* @param vertexAttributes The vertex attributes present in the data.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int vertexCount, VertexAttributeType vertexAttributes, BufferType bt);

	/**
	* Applies the attributes stored in this vertex buffer
	*/
	void ApplyAttributes();

	/**
	* Locks the buffer and returns a pointer to its memory
	*/
	void *Lock();

	/**
	* Unlocks the buffer
	*/
	void Unlock();

	/**
	* Destroys the buffer.
	*/
	void Destroy();
	
	/**
	* @returns The number of vertices stored in this buffer
	*/
	unsigned int GetVertexCount() const;

	/**
	* @returns The number of bytes used per vertex
	*/
	unsigned int GetVertexSize() const;

	/**
	* @returns An ordered list of the vertex attributes in this vertex buffer (item 0 corresponds to vertex atribute 0, etc..)
	*/
	const std::vector<VertexAttribute> &GetVertexAttributes() const;

	/**
	* @returns The offset (in bytes) for a given vertex attribute type
	* @param type The vertex attribute type
	*/
	unsigned int GetVertexAttributeOffset(VertexAttributeType type);

	/**
	* Adds a vertex attribute
	* @param type The type of the vertex attribute
	* @param dataType The data type of each element for the attribute (float, byte, ...)
	* @param count The number of element per vertex of this attribute
	*/
	void AddVertexAttribute(VertexAttributeType type, VertexAttributeDataType dataType, int count);

	/**
	* @returns Whether the vertex buffer contains an attribute of a give type
	* @param type The attribute type
	*/
	bool HasAttribute(VertexAttributeType type);

	/**
	* @returns the shadowed data if shadowing was enabled
	*/
	const std::vector<char> &GetShadowData() const;

	/**
	* Enabled / disables shadowing of the data. If shadowing is enabled, a copy of data is stored in local memory.
	*/
	void SetShadowed(bool isShadowed);

	/**
	* @returns Whether the buffer is shadowed
	*/
	const bool Shadowed() const;

	/**
	* @returns The vertex size in bytes for a given set of vertex attributes
	* @param vertexAttributes The vertex attributes present per vertex (or'ed together) 
	*/
	static unsigned int GetVertexSize(VertexAttributeType vertexAttributes);

	/**
	* @returns The attribute size in bytes of a specific type
	* @param vertexAttribute The vertex attribute type
	*/
	static unsigned int GetAttributeSize(VertexAttributeType vertexAttribute);

	/**
	* @returns The vertex attribute index for a specific vertex attribute type (position must be attriute 0, normal must be attribute 1, ...)
	*/
	static unsigned int GetVertexAttributeIndex(VertexAttributeType vertexAttribute);
private:
	static const unsigned int attributeSize[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)];
	static const VertexAttributeDataType attributeDataType[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)];
	void UpdateAttributesOffsets();
	
	std::vector<VertexAttribute> vertexAttributes;
	//VertexAttributeType vertexAttributes;
	//unsigned int vertexAttributesOffset[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)];
	unsigned int vertexSize;
	unsigned int vertexCount;
	bool isShadowed;
	std::vector<char> shadowData;
};

}
#pragma warning(pop)
