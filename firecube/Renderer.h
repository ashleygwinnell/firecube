#ifndef RENDERER_H
#define RENDERER_H

class Model;

enum BufferType
{
	STATIC,DYNAMIC,STREAM
};
enum RenderMode
{
POINTS,LINES,TRIANGLES,TRIANGLE_STRIP,QUADS,LINE_LOOP,TRIANGLE_FAN
};
class FIRECUBE_API Buffer
{
public:	
	Buffer();
	~Buffer();
	void Create();	
	void LoadData(void *data,DWORD size,BufferType bt);
	void LoadIndexData(void *data,DWORD count,BufferType bt);
	void SetVertexStream();
	void SetColorStream();
	void SetTexCoordStream(unsigned int unit);
	void RenderIndexStream(RenderMode mode,DWORD count);
	void Destroy();

	GLuint buffer;
};
class FIRECUBE_API Renderer
{
public:
	Renderer();
	~Renderer();
	void Clear();

	void Render(Model &model);
	void SetModelViewMatrix(mat4 &m);
	void SetProjectionMatrix(mat4 &m);
private:	
};

#endif