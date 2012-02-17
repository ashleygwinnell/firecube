#ifndef BUFFER_H
#define BUFFER_H

#pragma warning(push)
#pragma warning(disable:4251)

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

/**
* A buffer of data used for rendering.
*/
class FIRECUBE_API Buffer
{
public:
    Buffer();
    ~Buffer();

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
    bool LoadData(void *data, unsigned int size, BufferType bt);
    
	/**
    * Specific function to load index data.
    * @param data Pointer to the indices to be stored.
    * @param count Number of indices.
    * @param bt The buffer type.
    * @return true on success.
    */
    bool LoadIndexData(void *data, unsigned int count, BufferType bt);    
    
	/**
    * Binds the buffer as an index stream.
    */
    void SetIndexStream();
	
	/**
	* Binds the buffer as a vertex attribute.
	* @param index The vertex attribute index.
	* @param numCoords The number of coordinates each element of the buffer has.
	* @param stride The distance between the start of each element in bytes.
	* @param offset The offset in bytes where this attribute starts from the beginning of the buffer.
	*/
	void SetVertexAttribute(int index, int numCoords, int stride, int offset);    	
    
	/**
    * Returns whether the buffer is valid.
    */
    bool IsValid() const;
    
	/**
    * Destroys the buffer.
    */
    void Destroy();
    
	/**
    * Returns the resource id of the buffer.
    */
    unsigned int GetId() const;

private:

    GLuint id;
};

/**
* A shared pointer to a Buffer.
*/
typedef boost::shared_ptr<Buffer> BufferPtr;

}
#pragma warning(pop)
#endif