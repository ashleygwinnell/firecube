#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>
#include <string>
#include <map>

#include "ThirdParty/GLEW/glew.h"
#include "Utils/utils.h"
#include "Math/Math.h"
#include "Core/Object.h"
#include "Core/Memory.h"
#include "Rendering/RenderingTypes.h"
#include "Rendering/SceneView.h"

namespace FireCube
{

// Forward declarations.
class Application;
class Node;
class Geometry;
class RenderQueue;
class Material;
class FrameBuffer;
class Shader;
class Program;
class Technique;
class ProgramUniformsList;
class Texture;
class FontFace;
class vec2;
class vec3;
class vec4;
class mat3;
class mat4;
class Camera;
enum class PrimitiveType;
class VertexBuffer;
class ShaderTemplate;
class Light;
class RenderPath;
class RenderSurface;

#define MAX_RENDER_TARGETS 4

/**
* A class containing various functions responsible for the rendering pipeline.
*/
class FIRECUBE_API Renderer : public Object
{
	OBJECT(Renderer)
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
	void Clear(const vec4 &color, float depth, ClearBufferType buffers);

	/**
	* Binds the specified texture.
	* @param unit Specifies the texture unit.
	* @param texture The texture to bind	
	*/
	void UseTexture(unsigned int unit, const Texture *texture);

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
	void RenderStream(const PrimitiveType &primitiveType, unsigned int count, unsigned int offset = 0);

	/**
	* Uses a program.
	* @param program The program to use.
	*/
	void UseProgram(Program *program);

	/**
	* Uses a material.
	* @param program The program to apply the material to.
	* @param material The material to use.
	*/
	void UseMaterial(Material *material);
	
	/**
	* Restores the default render buffer.
	*/
	void RestoreFrameBuffer();

	/**
	* Sets the rendering viewport.
	*/
	void SetViewport(int left, int right, int width, int height);

	void IncreamentNumberOfPrimitivesRendered(unsigned int amount);

	/**
	* Returns the number of triangles that were rendered in the current frame.
	*/
	unsigned int GetNumberOfPrimitivesRendered();

	Program *SetShaders(Shader *vertexShader, Shader *fragmentShader); // TODO change to void

	void UseCamera(Camera *camera);

	void UseLight(Light *light);

	void ResetCachedShaderParameters();		

	void SetRenderTarget(unsigned int index, RenderSurface *renderTarget);

	void SetDepthSurface(RenderSurface *depthSurface);
	
	RenderPath *GetDefaultRenderPath();

	void UpdateFrameBuffer();

	void RenderFullscreenQuad();

	void SetWidth(int width);
	void SetHeight(int height);

	int GetWidth() const;
	int GetHeight() const;

	SharedPtr<RenderSurface> GetRenderSurface(int width, int height, RenderSurfaceType type);

	void SetBlendMode(BlendMode blendMode);
	void SetDepthWrite(bool depthWrite);
	void SetDepthTest(DepthTest depthTest);
	void SetTimeStep(float timeStep);
	float GetTimeStep() const;
	RenderSurface *GetShadowMap();
	void SetBuffer(VertexBuffer *vertexBuffer);
	void SetSceneView(unsigned int index, SceneView *sceneView);
	void Render();
private:

	/**
	* Uses a frame buffer.
	* @param frameBuffer The frame buffer to use.
	*/
	void UseFrameBuffer(FrameBuffer *frameBuffer);	

	VertexBuffer *quadVertexBuffer;	
	GLuint quadVao, dummyVao;
	
	std::map<std::pair<Shader *, Shader *>, Program *> programs;
	Program *currentProgram;	
	Shader *currentVertexShader;
	Shader *currentFragmentShader;
	Material *currentMaterial;
	Camera *currentCamera;
	Light *currentLight;
	unsigned int numberOfPrimitivesRendered;
	GLuint textureSampler[16];	
	RenderSurface *renderTargets[MAX_RENDER_TARGETS];
	RenderSurface *depthSurface;
	std::map<unsigned int, FrameBuffer *> frameBuffers;
	std::map<long long int, SharedPtr<RenderSurface>> renderSurfaces;
	FrameBuffer *currentFrameBuffer;
	RenderSurface *shadowMap;
	bool fboDirty;	
	SharedPtr<RenderPath> defaultRenderPath;
	int width, height;
	BlendMode blendMode;
	bool depthWrite;
	DepthTest depthTest;
	float timeStep;
	std::vector<SharedPtr<SceneView>> sceneViews;
};

}
#pragma warning(pop)
