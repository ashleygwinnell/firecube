#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Rendering/GraphicsResource.h"
#include "VertexBuffer.h"

namespace FireCube
{

/**
* A buffer of indices used for rendering.
*/
class FIRECUBE_API IndexBuffer : public GraphicsResource
{
public:
	IndexBuffer(Renderer *renderer);
	~IndexBuffer();

	/**
	* Creates a new buffer.
	*/
	void Create();
	
	/**
	* Loads indices into the buffer.
	* @param data Pointer to the indices to be stored.
	* @param count Number of indices.
	* @param bt The buffer type.
	* @return true on success.
	*/
	bool LoadData(void *data, unsigned int count, BufferType bt);    
	
	/**
	* Binds the buffer as an index stream.
	*/
	void SetIndexStream();		
	
	/**
	* Destroys the buffer.
	*/
	void Destroy();

	const std::vector<char> &GetShadowData() const;
	void SetShadowed(bool isShadowed);
	const bool Shadowed() const;
	unsigned int GetIndicesCount() const;
private:
	bool isShadowed;
	std::vector<char> shadowData;	
	unsigned int indicesCount;
};

/**
* A shared pointer to a Buffer.
*/
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;

}
#pragma warning(pop)