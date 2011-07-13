#ifndef RENDERER_H
#define RENDERER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
template<class T>
class ResourceManager;
class Application;
class Node;
typedef boost::shared_ptr<Node> NodePtr;
class Geometry;
typedef boost::shared_ptr<Geometry> GeometryPtr;
class RenderQueue;
class Material;
typedef boost::shared_ptr<Material> MaterialPtr;
class FrameBuffer;
typedef boost::shared_ptr<FrameBuffer> FrameBufferPtr;
class Shader;
typedef ResourceManager<Shader> ShaderManager;
class Program;
typedef boost::shared_ptr<Program> ProgramPtr;
class Technique;
typedef boost::shared_ptr<Technique> TechniquePtr;
class ProgramUniformsList;
class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;
typedef ResourceManager<Texture> TextureManager;
class Font;
typedef boost::shared_ptr<Font> FontPtr;
class FontManager;
class vec2;
class vec3;
class vec4;
class mat3;
class mat4;
class Camera;
typedef boost::shared_ptr<Camera> CameraPtr;
enum PrimitiveType;

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
    * Generates the rendering state from a material.
    * @param mat The material to generate the state from.
    */
    void FromMaterial(MaterialPtr mat);

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
	* Can be an empty program. If so the technique is used to generate one.
    */
    ProgramPtr program;

    /**
    * The technique used for rendering.
    */
    TechniquePtr technique;

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
void FIRECUBE_API Clear(const vec4 &color, float depth);

/**
* Renders a geometry.
* @param geometry The geometry to render.
*/
void FIRECUBE_API Render(GeometryPtr geometry);

/**
* Renders a scene node.
* @param node The node to render.
*/
void FIRECUBE_API Render(NodePtr node);

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
void FIRECUBE_API Render(NodePtr node, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);

/**
* Renders a render queue using a specified technique.
* @param renderQueue The render queue to render.
* @param techniqueName The name of the technique.
* @param programUniformsList A list of values to assign to uniform variables of generated programs.
*/
void FIRECUBE_API Render(RenderQueue &renderQueue, const std::string &techniqueName, const ProgramUniformsList &programUniformsList);


/**
* Sets the current modelview matrix.
*/
void FIRECUBE_API SetModelViewMatrix(const mat4 &m);

/**
* Sets the current projection matrix.
*/
void FIRECUBE_API SetProjectionMatrix(const mat4 &m);

/**
* Sets the a texture matrix.
*/
void FIRECUBE_API SetTextureMatrix(const mat4 &m, int unit);

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
void FIRECUBE_API MultiplyModelViewMatrix(const mat4 &mat);

/**
* Binds the specified texture.
* @param tex The texture to bind
* @param unit Specifies the texture unit.
*/
void FIRECUBE_API UseTexture(TexturePtr tex, unsigned int unit);

/**
* Renders text.
* @param font The font to use.
* @param pos The position of the text.
* @param color The color of the text.
* @param str The string to output.
*/
void FIRECUBE_API RenderText(FontPtr font, const vec3 &pos, const vec4 &color, const std::string &str);

/**
* Renders an index stream.
* @param primitiveType The primitive type.
* @param count The number of indices to render.
*/
void FIRECUBE_API RenderIndexStream(const PrimitiveType &primitiveType, unsigned int count);

/**
* Renders a stream.
* @param primitiveType The primitive type.
* @param count The number of indices to render.
*/
void FIRECUBE_API RenderStream(const PrimitiveType &primitiveType, unsigned int count);

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
void FIRECUBE_API UseProgram(ProgramPtr program);

/**
* Uses a material.
* @param material The material to use.
*/
void FIRECUBE_API UseMaterial(MaterialPtr material);

/**
* Uses a frame buffer.
* @param frameBuffer The frame buffer to use.
*/
void FIRECUBE_API UseFrameBuffer(FrameBufferPtr frameBuffer);

/**
* Restores the default render buffer.
*/
void FIRECUBE_API RestoreFrameBuffer();

/**
* Sets the rendering viewport.
*/
void FIRECUBE_API SetViewport(int left, int right, int width, int height);

/**
* Sets a perspective projection.
* @param fov The field of view in degrees.
* @param zNear The near clipping plane.
* @param zFar The far clipping plane.
*/
void FIRECUBE_API SetPerspectiveProjection(float fov, float zNear, float zFar);

/**
* Sets an orthographic projection with a one to one pixel ratio.
*/
void FIRECUBE_API SetOrthographicProjection();

/**
* Adds a technique to the list of techniques.
* @param name The name of the technique.
* @param technique The technique to associate with name.
*/
void FIRECUBE_API AddTechnique(const std::string &name, TechniquePtr technique);

/**
* Get a technique by name.
* @param name The name of the technique.
*/
TechniquePtr FIRECUBE_API GetTechnique(const std::string &name);

/**
* Removes a technique.
* @param name The name of the technique.
*/
void FIRECUBE_API RemoveTechnique(const std::string &name);

/**
* Sets the current camera.
* @param camera The camera to set.
*/
void FIRECUBE_API SetCamera(CameraPtr camera);

/**
* gets the current camera.
* @return The current camera.
*/
CameraPtr FIRECUBE_API GetCamera();

/**
* Returns the number of triangles that were in the current frame.
*/
unsigned int FIRECUBE_API GetNumberOfTrianglesRendered();
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