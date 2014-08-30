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
enum BufferType
{
	STATIC, DYNAMIC, STREAM
};

enum class FIRECUBE_API VertexAttributeType
{
	POSITION = 1, NORMAL = 2, TEXCOORD0 = 4, TANGENT = 8, COLOR = 16, MAX_VERTEX_ATTRIBUTE = 5	
};

VertexAttributeType operator | (const VertexAttributeType &lhs, const VertexAttributeType &rhs);
VertexAttributeType operator |= (VertexAttributeType &lhs, const VertexAttributeType &rhs);
VertexAttributeType operator & (const VertexAttributeType &lhs, const VertexAttributeType &rhs);

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
	* @param size Size of the data in bytes.
	* @param bt The buffer type.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int vertexCount, VertexAttributeType vertexAttributes, BufferType bt);
	
	/**
	* Binds the buffer as a vertex attribute.
	* @param index The vertex attribute index.
	* @param numCoords The number of coordinates each element of the buffer has.
	* @param stride The distance between the start of each element in bytes.
	* @param offset The offset in bytes where this attribute starts from the beginning of the buffer.
	*/
	void SetVertexAttribute(int index, int numCoords, int stride, int offset);    		
	void ApplyAttributes();
	void *Lock();
	void Unlock();
	/**
	* Destroys the buffer.
	*/
	void Destroy();
	
	unsigned int GetVertexCount() const;
	unsigned int GetVertexSize() const;
	VertexAttributeType GetVertexAttributes() const;
	unsigned int GetVertexAttributeOffset(VertexAttributeType vertexAttribute);

	const std::vector<char> &GetShadowData() const;
	void SetShadowed(bool isShadowed);
	const bool Shadowed() const;

	static unsigned int GetVertexSize(VertexAttributeType vertexAttributes);
	static unsigned int GetAttributeSize(VertexAttributeType vertexAttribute);
	static unsigned int GetVertexAttributeIndex(VertexAttributeType vertexAttribute);
private:
	static const unsigned int attributeSize[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)];
	void UpdateAttributesOffsets();
	
	VertexAttributeType vertexAttributes;
	unsigned int vertexAttributesOffset[static_cast<int>(VertexAttributeType::MAX_VERTEX_ATTRIBUTE)];
	unsigned int vertexSize;
	unsigned int vertexCount;
	bool isShadowed;
	std::vector<char> shadowData;
};

}
#pragma warning(pop)
