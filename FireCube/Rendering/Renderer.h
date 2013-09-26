#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <map>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Core/Object.h"

namespace FireCube
{

// Forward declarations.
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
typedef std::shared_ptr<Shader> ShaderPtr;
class Program;
typedef std::shared_ptr<Program> ProgramPtr;
class Technique;
typedef std::shared_ptr<Technique> TechniquePtr;
class ProgramUniformsList;
class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
class FontFace;
typedef std::shared_ptr<FontFace> FontFacePtr;
class vec2;
class vec3;
class vec4;
class mat3;
class mat4;
class Camera;
typedef std::shared_ptr<Camera> CameraPtr;
enum PrimitiveType;
class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
class ShaderTemplate;
class Light;

/**
* A class containing various functions responsible for the rendering pipeline.
*/
class FIRECUBE_API Renderer : public Object
{
public:

	Renderer(Engine *engine);

	void ResetNumberOfPrimitivesRendered();	
	void Initialize();
	void Destroy();

	/**
	* Clears the depth and color buffer.
	* @param color The color to clear the framebuffer.
	* @param depth The depth to set the depth buffer.
	*/
	void Clear(const vec4 &color, float depth);

	/**
	* Binds the specified texture.
	* @param tex The texture to bind
	* @param unit Specifies the texture unit.
	*/
	void UseTexture(TexturePtr tex, unsigned int unit);

	/**
	* Renders text.
	* @param font The font to use.
	* @param pos The position of the text.
	* @param color The color of the text.
	* @param str The string to output.
	*/
	void RenderText(FontFacePtr fontFace, CameraPtr camera, const vec3 &pos, const vec4 &color, const std::string &str);

	/**
	* Renders an index stream.
	* @param primitiveType The primitive type.
	* @param count The number of indices to render.
	*/
	void RenderIndexStream(const PrimitiveType &primitiveType, unsigned int count);

	/**
	* Renders a stream.
	* @param primitiveType The primitive type.
	* @param count The number of indices to render.
	*/
	void RenderStream(const PrimitiveType &primitiveType, unsigned int count);

	/**
	* Disables a texture coordinates stream.
	* @param unit The texture unit.
	*/
	void DisableTexCoordStream(unsigned int unit);

	/**
	* Disables the normal stream.
	*/
	void DisableNormalStream();

	/**
	* Uses a program.
	* @param program The program to use.
	*/
	void UseProgram(ProgramPtr program);

	/**
	* Uses a material.
	* @param program The program to apply the material to.
	* @param material The material to use.
	*/
	void UseMaterial(Material *material);

	/**
	* Uses a frame buffer.
	* @param frameBuffer The frame buffer to use.
	*/
	void UseFrameBuffer(FrameBufferPtr frameBuffer);

	/**
	* Restores the default render buffer.
	*/
	void RestoreFrameBuffer();

	/**
	* Sets the rendering viewport.
	*/
	void SetViewport(int left, int right, int width, int height);

	void IncreamentNumboerOfPrimitivesRendered(unsigned int amount);

	/**
	* Returns the number of triangles that were rendered in the current frame.
	*/
	unsigned int GetNumberOfPrimitivesRendered();


	/**
	* Disabled a vertex attribute.
	* @param index The index of the attribute.
	*/
	void DisableVertexAttribute(int index);

	ProgramPtr SetShaders(Shader *vertexShader, Shader *fragmentShader); // TODO change to void

	void UseCamera(Camera *camera);

	void UseLight(Light *light);
private:
	VertexBufferPtr textVertexBuffer;
	GLuint textVao;
	ShaderPtr textVertexShader;
	ShaderPtr textFragmentShader;

	std::map<std::pair<Shader *, Shader *>, ProgramPtr> programs;
	ProgramPtr currentProgram;	
	Shader *currentVertexShader;
	Shader *currentFragmentShader;
	Material *currentMaterial;
	Camera *currentCamera;
	Light *currentLight;
	unsigned int numberOfPrimitivesRendered;
	GLuint textureSampler[16];
};

}
#pragma warning(pop)
