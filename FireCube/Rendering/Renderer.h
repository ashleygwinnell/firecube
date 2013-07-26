#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>

#include "Utils/utils.h"
#include "Math/MyMath.h"

namespace FireCube
{

// Forward declarations.
template<class T>
class ResourcePool;
class Application;
class Node;
typedef std::shared_ptr<Node> NodePtr;
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;
class RenderQueue;
class Material;
typedef std::shared_ptr<Material> MaterialPtr;
class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
class Shader;
typedef ResourcePool<Shader> ShaderPool;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;
class Technique;
typedef std::shared_ptr<Technique> TechniquePtr;
class ProgramUniformsList;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
typedef ResourcePool<Texture> TexturePool;
class Font;
typedef std::shared_ptr<Font> FontPtr;
class FontPool;
class vec2;
class vec3;
class vec4;
class mat3;
class mat4;
class Camera;
typedef std::shared_ptr<Camera> CameraPtr;
enum PrimitiveType;

/**
* A class storing information about rendering properties.
* This class is used when generating a Program from a Technique.
*/
class FIRECUBE_API ShaderProperties
{
	friend class Technique;
public:
	ShaderProperties();

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
	* Sets whether diffuse texture should be used.
	* @param enable True to enable, false to disable.
	*/
	void SetDiffuseTexture(bool enable);

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
* @param program The program to apply the material to.
* @param material The material to use.
*/
void FIRECUBE_API UseMaterial(ProgramPtr program, MaterialPtr material);

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
* Uses the passed camera for consequent rendering calls.
* @param camera The camera to use.
*/
void FIRECUBE_API UseCamera(CameraPtr camera);

/**
* gets the current camera.
* @return The current camera.
*/
CameraPtr FIRECUBE_API GetCamera();

/**
* Returns the number of triangles that were rendered in the current frame.
*/
unsigned int FIRECUBE_API GetNumberOfPrimitivesRendered();


/**
* Disabled a vertex attribute.
* @param index The index of the attribute.
*/
void FIRECUBE_API DisableVertexAttribute(int index);

/**
* Sets the current texture pool.
*/
void FIRECUBE_API SetTexturePool(TexturePool &texturePool);

/**
* Gets the current texture pool.
*/
TexturePool FIRECUBE_API &GetTexturePool();

/**
* Sets the current shader pool.
*/
void FIRECUBE_API SetShaderPool(ShaderPool &shaderPool);

/**
* Gets the current shader pool.
*/
ShaderPool FIRECUBE_API &GetShaderPool();

/**
* Sets the current font pool.
*/
void FIRECUBE_API SetFontPool(FontPool &fonPool);

/**
* Gets the current font pool.
*/
FontPool FIRECUBE_API &GetFontPool();
};
}
#pragma warning(pop)
