#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

class Application;
/**
* A namespace containing various functions responsible for the rendering pipeline.
*/
namespace Renderer
{
	/**
	* Clears the depth and color buffer.
	* @param color The color to clear the framebuffer.
	* @depth The depth to set the depth buffer.
	*/
	void FIRECUBE_API Clear(vec4 color,float depth);
	/**
	* Renders a model.
	* @param model The model to render.
	*/
	void FIRECUBE_API Render(Model model);
	/**
	* Sets the current modelview matrix.
	*/
	void FIRECUBE_API SetModelViewMatrix(mat4 &m);
	/**
	* Sets the current projection matrix.
	*/
	void FIRECUBE_API SetProjectionMatrix(mat4 &m);
	/**
	* Pushes the the current modelview matrix to the stack.
	*/
	void FIRECUBE_API SaveModelViewMatrix();
	/**
	* Pops the top modelview matrix from the stack and sets it to be the current matrix.
	*/
	void FIRECUBE_API RestoreModelViewMatrix();
	/**
	* Pushes the the current projection matrix to the stack.
	*/
	void FIRECUBE_API SaveProjectionMatrix();
	/**
	* Pops the top projection matrix from the stack and sets it to be the current matrix.
	*/
	void FIRECUBE_API RestoreProjectionMatrix();
	/**
	* Multiplies the current modelview matrix by a given matrix.	
	*/
	void FIRECUBE_API MultiplyModelViewMatrix(mat4 &mat);
	/**
	* Binds the specified texture.
	* @param texture The texture to bind
	* @param unit Specifies the texture unit.
	*/
	void FIRECUBE_API UseTexture(Texture tex,unsigned int unit);
	/**
	* Renders text.
	* @param font The font to use.
	* @param pos The position of the text.
	* @param str The string to output.
	*/
	void FIRECUBE_API RenderText(Font font,vec2 pos,vec4 color,const string &str);
	/**
	* Renders an index stream.	
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void FIRECUBE_API RenderIndexStream(RenderMode mode,DWORD count);
	/**
	* Renders a stream.
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void FIRECUBE_API RenderStream(RenderMode mode,DWORD count);
	/**
	* Uses a program.
	* @param program The program to use.
	*/
	void FIRECUBE_API UseProgram(Program program);
	/**
	* Uses a material.
	* @param material The material to use.
	*/
	void FIRECUBE_API UseMaterial(Material material);
	/**
	* Uses a frame buffer.
	* @param frameBuffer The frame buffer to use.
	*/
	void FIRECUBE_API UseFrameBuffer(FrameBuffer frameBuffer);
	/**
	* Restores the default render buffer.
	*/
	void FIRECUBE_API RestoreFrameBuffer();
	/**
	* Sets the rendering viewport.
	*/
	void FIRECUBE_API SetViewport(int width,int height);
	/**
	* Sets a perspective projection.
	* @param fov The field of view in degrees.
	* @param zNear The near clipping plane.
	* @param zFar The far clipping plane.
	*/
	void FIRECUBE_API SetPerspectiveProjection(float fov,float zNear,float zFar);	
	/**
	* Sets an orthographic projection with a one to one pixel ratio.	
	*/
	void FIRECUBE_API SetOrthographicProjection();
	/**
	* Sets the current texture manager.
	*/
	void FIRECUBE_API SetTextureManager(TextureManager *textureManager);
	/**
	* Gets the current texture manager.
	*/
	TextureManager FIRECUBE_API *GetTextureManager();
	/**
	* Sets the current shader manager.
	*/
	void FIRECUBE_API SetShaderManager(ShaderManager *shaderManager);
	/**
	* Gets the current shader manager.
	*/
	ShaderManager FIRECUBE_API *GetShaderManager();
	/**
	* Sets the current font manager.
	*/
	void FIRECUBE_API SetFontManager(FontManager *fontManager);
	/**
	* Gets the current font manager.
	*/
	FontManager FIRECUBE_API *GetFontManager();
};


#pragma warning(pop)

#endif