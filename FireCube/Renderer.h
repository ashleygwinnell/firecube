#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
	class Application;
	class Node;
	class RenderQueue;

/**
* A class storing information about rendering properties.
* This class is used when generating a program.
*/
class FIRECUBE_API RenderState
{	
	friend class Technique;
public:
	RenderState();

	/**
	* Generates the rendering from a material.
	* @param mat The material to generate the state from.
	*/
	void FromMaterial(Material mat);

	/**
	* Sets whether directional lighting should be used.
	* @param enable True to enable, false to disable.
	*/
	void SetDirectionalLighting(bool enable);

	/**
	* Sets whether point lighting should be used.
	* @param enable True to enable, false to disable.
	*/
	void SetPointLighting(bool enable);

	/**
	* Sets whether fog should be used.
	* @param enable True to enable, false to disable.
	*/
	void SetFog(bool enable);

	/**
	* Hashes the rendering state.
	* @return An integer representing this rendering state.
	*/
	unsigned int ToInt() const;
private:
	bool directionalLighting;
	bool pointLighting;
	bool diffuseTexture;
	bool normalTexture;
	bool fog;
};

/**
* A class holding information on various rendering parameters.
*/
class FIRECUBE_API RenderParameters
{
public:
	/**
	* The program used for rendering.
	*/
	Program program;

	/**
	* The technique used for rendering.
	*/
	Technique technique;

	/**
	* Specifies whether dynamic lighting is enabled.
	*/
	bool lighting;

	/**
	* Specifies whether fog is enabled.
	*/
	bool fog;

	/**
	* The fog color.
	*/
	vec4 fogColor;

	/**
	* The fog density.
	*/
	float fogDensity;	
};

/**
* A namespace containing various functions responsible for the rendering pipeline.
*/
namespace Renderer
{
	/**
	* Clears the depth and color buffer.
	* @param color The color to clear the framebuffer.
	* @param depth The depth to set the depth buffer.
	*/
	void FIRECUBE_API Clear(vec4 color,float depth);
	
	/**
	* Renders a geometry.
	* @param geometry The geometry to render.
	*/
	void FIRECUBE_API Render(Geometry geometry);
	
	/**
	* Renders a scene node.
	* @param node The node to render.
	*/
	void FIRECUBE_API Render(Node node);

	/**
	* Renders a render queue.
	* @param renderQueue The render queue to render.
	*/
	void FIRECUBE_API Render(RenderQueue &renderQueue);

	/**
	* Renders a scene node using a specified technique.
	* @param node The node to render.
	* @param techniqueName The name of the technique.
	* @param programUniformsList A list of values to assign to uniform variables of generated programs.
	*/
	void FIRECUBE_API Render(Node node, const std::string &techniqueName, ProgramUniformsList &programUniformsList);
	
	/**
	* Sets the current modelview matrix.
	*/
	void FIRECUBE_API SetModelViewMatrix(mat4 &m);
	
	/**
	* Sets the current projection matrix.
	*/
	void FIRECUBE_API SetProjectionMatrix(mat4 &m);
	
	/**
	* Sets the a texture matrix.
	*/
	void FIRECUBE_API SetTextureMatrix(mat4 &m,int unit);
	
	/**
	* Returns the current modelview matrix.
	*/
	mat4 FIRECUBE_API GetModelViewMatrix();
	
	/**
	* Returns the current projection matrix.
	*/
	mat4 FIRECUBE_API GetProjectionMatrix();
	
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
	* @param tex The texture to bind
	* @param unit Specifies the texture unit.
	*/
	void FIRECUBE_API UseTexture(Texture tex,unsigned int unit);
	
	/**
	* Renders text.
	* @param font The font to use.
	* @param pos The position of the text.
	* @param color The color of the text.
	* @param str The string to output.
	*/
	void FIRECUBE_API RenderText(Font font,vec3 pos,vec4 color,const std::string &str);
	
	/**
	* Renders an index stream.	
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void FIRECUBE_API RenderIndexStream(RenderMode mode,unsigned int count);
	
	/**
	* Renders a stream.
	* @param mode The rendering mode.
	* @param count The number of indices to render.
	*/
	void FIRECUBE_API RenderStream(RenderMode mode,unsigned int count);
	
	/**
	* Disables a texture coordinates stream.
	* @param unit The texture unit.
	*/
	void FIRECUBE_API DisableTexCoordStream(unsigned int unit);
	
	/**
	* Disables the normal stream.	
	*/
	void FIRECUBE_API DisableNormalStream();
	
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
	void FIRECUBE_API SetViewport(int left,int right,int width,int height);
	
	/**
	* Sets a global program overriding any program assignment.
	* @param program The program to use
	*/
	void FIRECUBE_API SetGlobalProgram(Program program);
	
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
	* Adds a technique to the list of techniques.
	* @param name The name of the technique.
	* @param technique The technique to associate with name.
	*/
	void FIRECUBE_API AddTechnique(const std::string &name, Technique technique);
	
	/**
	* Get a technique by name.
	* @param name The name of the technique.
	*/
	Technique FIRECUBE_API GetTechnique(const std::string &name);
	
	/**
	* Removes a technique.
	* @param name The name of the technique.
	*/
	void FIRECUBE_API RemoveTechnique(const std::string &name);
	
	/**
	* Sets the current texture manager.
	*/
	void FIRECUBE_API SetTextureManager(TextureManager &textureManager);
	
	/**
	* Gets the current texture manager.
	*/
	TextureManager FIRECUBE_API &GetTextureManager();
	
	/**
	* Sets the current shader manager.
	*/
	void FIRECUBE_API SetShaderManager(ShaderManager &shaderManager);
	
	/**
	* Gets the current shader manager.
	*/
	ShaderManager FIRECUBE_API &GetShaderManager();
	
	/**
	* Sets the current font manager.
	*/
	void FIRECUBE_API SetFontManager(FontManager &fontManager);
	
	/**
	* Gets the current font manager.
	*/
	FontManager FIRECUBE_API &GetFontManager();
};
}
#pragma warning(pop)

#endif