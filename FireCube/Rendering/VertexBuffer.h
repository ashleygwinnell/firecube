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
	* @param bt The buffer type.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int vertexCount, BufferType bt);

	/**
	* Loads the specified data to the buffer.
	* @param data Pointer to the data to be stored.
	* @param size Size of the data in bytes.
	* @param bt The buffer type.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int vertexCount, VertexAttributeType vertexAttributes, BufferType bt);

	void ApplyAttributes();
	void *Lock();
	void Unlock();
	/**
	* Destroys the buffer.
	*/
	void Destroy();
	
	unsigned int GetVertexCount() const;
	unsigned int GetVertexSize() const;
	const std::vector<VertexAttribute> &GetVertexAttributes() const;
	unsigned int GetVertexAttributeOffset(VertexAttributeType type);
	void AddVertexAttribute(VertexAttributeType type, VertexAttributeDataType dataType, int count);
	bool HasAttribute(VertexAttributeType type);

	const std::vector<char> &GetShadowData() const;
	void SetShadowed(bool isShadowed);
	const bool Shadowed() const;

	static unsigned int GetVertexSize(VertexAttributeType vertexAttributes);
	static unsigned int GetAttributeSize(VertexAttributeType vertexAttribute);
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
