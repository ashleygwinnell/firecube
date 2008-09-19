#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

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
	* @param color The color to clear the framebuffer.
	* @depth The depth to set the depth buffer.
	*/
	void Clear(vec4 color,float depth);
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
	/**
	* Uses a program.
	* @param program The program to use.
	*/
	void UseProgram(const Program &program);
	/**
	* Uses a material.
	* @param material The material to use.
	*/
	void UseMaterial(const Material &material);
	/**
	* Sets a perspective projection.
	* @param fov The field of view in degrees.
	* @param zNear The near clipping plane.
	* @param zFar The far clipping plane.
	*/
	void SetPerspectiveProjection(float fov,float zNear,float zFar);	
	/**
	* Sets an orthographic projection with a one to one pixel ratio.	
	*/
	void SetOrthographicProjection();
private:	
	Program textShader;
};


#pragma warning(pop)

#endif