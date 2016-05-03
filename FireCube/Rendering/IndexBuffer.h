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
	* @returns The number of indices stored in this buffer
	*/
	unsigned int GetIndicesCount() const;
private:
	bool isShadowed;
	std::vector<char> shadowData;	
	unsigned int indicesCount;
};

}
#pragma warning(pop)