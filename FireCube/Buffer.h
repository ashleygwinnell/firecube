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
* Specifies the kind of primitives to render.
*/
enum RenderMode
{
    POINTS, LINES, TRIANGLES, TRIANGLE_STRIP, QUADS, LINE_LOOP, TRIANGLE_FAN
};

/**
* A buffer of data used for rendering.
*/
class FIRECUBE_API BufferResource
{
public:
    BufferResource();
    ~BufferResource();

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
    * Binds the buffer as a vertex stream.
    * @param numCoords The number of coordinates per vertex.
    */
    void SetVertexStream(int numCoords);
    /**
    * Binds the buffer as a normal stream.
    */
    void SetNormalStream();
    /**
    * Binds the buffer as a color stream.
    */
    void SetColorStream();
    /**
    * Binds the buffer as a texture coordinate stream.
    * @param unit Specifies the texture unit.
    */
    void SetTexCoordStream(unsigned int unit);
    /**
    * Binds the buffer as an index stream.
    */
    void SetIndexStream();
    /**
    * Binds the buffer.
    */
    void Bind();
    /**
    * Returns whether the buffer is valid.
    */
    bool IsValid();
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
* A shared pointer to a BufferResource.
*/
typedef boost::shared_ptr<BufferResource> Buffer;

}
#pragma warning(pop)
#endif