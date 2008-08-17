#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

/**
* A class Representing a single shader.
*/
class FIRECUBE_API ShaderResource
{
public:
	ShaderResource();
	~ShaderResource();
	/**
	* Loads a shader from a file. the shader type is determined by the extension of the file: .vshader for a vertex shader, .pshader for a pixel shader.
	* @param filename The file to load.
	*/
	bool Load(const string &filename);
	GLuint id;
};

typedef boost::shared_ptr<ShaderResource> Shader;
typedef ResourceManager<ShaderResource> ShaderManager;

/**
* A class representing a gpu program.
*/
class FIRECUBE_API Program
{
public:
	Program();
	~Program();
	/**
	* Creates the program.
	*/
	void Create();
	/**
	* Creates the program and attaches two shaders to it.
	*/
	void Create(Shader shader1,Shader shader2);
	/**
	* Attaches a shader to the program.
	* @param shader The shader to attach.
	*/
	void Attach(Shader shader);
	/**
	* Links the program.
	*/
	void Link();
	/**
	* Uses the program.
	* @param useShaders Default to true. when set to false the rendering reverts to the fixed functionality pipeline.
	*/
	void Use(bool useShaders=true);
	/**
	* Sets a 1d uniform float.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void Uniform1f(const string &name,float value);
	/**
	* Sets a 1d uniform integer.
	* @param name The name of the variable.
	* @param value The value to assign for it.
	*/
	void Uniform1i(const string &name,int value);	
	GLuint id;
	map<string,GLint> variables;
};

/**
* A class responsible for the rendering pipeline.
*/
class FIRECUBE_API Renderer
{
public:
	Renderer();
	~Renderer();
	/**
	* Clears the depth and color buffer.
	*/
	void Clear();
	/**
	* Renders a model.
	* @param model The model to render.
	*/
	void Render(Model model);
	/**
	* Sets the current modelview matrix.
	*/
	void SetModelViewMatrix(mat4 &m);
	/**
	* Sets the current projection matrix.
	*/
	void SetProjectionMatrix(mat4 &m);
	/**
	* Pushes the the current modelview matrix to the stack.
	*/
	void SaveModelViewMatrix();
	/**
	* Pops the top modelview matrix from the stack and sets it to be the current matrix.
	*/
	void RestoreModelViewMatrix();
	/**
	* Multiplies the current modelview matrix by a given matrix.	
	*/
	/**
	* Pushes the the current projection matrix to the stack.
	*/
	void SaveProjectionMatrix();
	/**
	* Pops the top projection matrix from the stack and sets it to be the current matrix.
	*/
	void RestoreProjectionMatrix();
	/**
	* Multiplies the current modelview matrix by a given matrix.	
	*/
	void MultiplyModelViewMatrix(mat4 &mat);
	/**
	* Binds the specified texture.
	* @param texture The texture to bind
	* @param unit Specifies the texture unit.
	*/
	void UseTexture(Texture tex,unsigned int unit);
	/**
	* Renders text.
	* @param font The font to use.
	* @param pos The position of the text.
	* @param str The string to output.
	*/
	void RenderText(Font font,vec2 pos,const string &str);
	/**
	* Renders an index stream.
	* @param buffer A buffer containing an index stream.
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void RenderIndexStream(RenderMode mode,DWORD count);
	/**
	* Renders a stream.
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void RenderStream(RenderMode mode,DWORD count);
private:	
};


#pragma warning(pop)

#endif