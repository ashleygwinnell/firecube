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
	FIRECUBE_OBJECT(Renderer)
public:

	Renderer(Engine *engine);

	/**
	* Resets the internal counter of the number of primitives that were renderered
	*/
	void ResetNumberOfPrimitivesRendered();
	
	/**
	* Initializes the renderer
	*/	
	void Initialize();
	
	/**
	* Destorys the renderer
	*/
	void Destroy();

	/**
	* Clears the depth and / or color buffer.
	* @param color The color to clear the framebuffer.
	* @param depth The depth to set the depth buffer.
	* @param buffers Controls which buffer to clear
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
	* @param offset An offset to start rendering from
	*/
	void RenderStream(const PrimitiveType &primitiveType, unsigned int count, unsigned int offset = 0);

	/**
	* Uses a program.
	* @param program The program to use.
	*/
	void UseProgram(Program *program);

	/**
	* Uses a material.	
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
	
	/**
	* Increaments the internal counter of the number of primitives that were renderered
	* @param amount The number of primitives to add
	*/
	void IncreamentNumberOfPrimitivesRendered(unsigned int amount);

	/**
	* Returns the number of primitives that were rendered in the current frame.
	*/
	unsigned int GetNumberOfPrimitivesRendered();
	
	/**
	* Sets the current shaders
	* @param vertexShader The vertex shader to use
	* @param fragmentShader The fragment shader to use
	* @returns A program generated from the supplied shaders
	*/
	Program *SetShaders(Shader *vertexShader, Shader *fragmentShader); // TODO change to void

	/**
	* Uses a camera.	
	* @param camera The camera to use.
	*/
	void UseCamera(Camera *camera);

	/**
	* Uses a light.	
	* @param light The light to use.
	*/
	void UseLight(Light *light);

	/**
	* Resets cached shader parameters state. After calling this function, all shader parameters (view matrix, light properties, ...) are invalidatd
	*/
	void ResetCachedShaderParameters();		

	/**
	* Sets the render target to use at a given index
	* @param index The index of the render target
	* @param renderTarget The render surface to use
	*/
	void SetRenderTarget(unsigned int index, RenderSurface *renderTarget);

	/**
	* Sets the depth render surface
	* @param depthSurface The render surface to use as the depth buffer
	*/
	void SetDepthSurface(RenderSurface *depthSurface);
	
	/**
	* @retruens The defualt render path
	*/
	RenderPath *GetDefaultRenderPath();

	/**
	* Updates the current framebuffer according to the currently set color / depth render targets. If no render targets were 
	* specified (using SetRenderTarget / SetDepthSurface) the default back buffer is used 	
	*/
	void UpdateFrameBuffer();

	/**
	* Renders a full screen quad
	*/
	void RenderFullscreenQuad();

	void SetWidth(int width);
	void SetHeight(int height);

	int GetWidth() const;
	int GetHeight() const;

	/**
	* Gets or creates a render surface. If uniqueKey is 0, an exisiting render surface with the same properties is returned (if one exists). Otherwise,
	* the uniqueKey is used to distinguish between render surfaces with the same properties
	* @param width The width of the render surface
	* @param height The height of the render surface
	* @param type The type of the render surface
	* @param uniqueKey A uniqueKey used to distinguish between render surfaces
	* @returns The render surface
	*/
	SharedPtr<RenderSurface> GetRenderSurface(int width, int height, RenderSurfaceType type, unsigned int uniqueKey = 0);

	/**
	* Sets the current blending mode
	* @param blendMode The blend mode to set
	*/
	void SetBlendMode(BlendMode blendMode);

	/**
	* Sets whether to enable depth writes
	* @param depthWrite Boolean specifiying whether to enabled depth writes
	*/
	void SetDepthWrite(bool depthWrite);

	/**
	* Sets the current depth test mode
	* @param depthTest The depth test mode to set
	*/
	void SetDepthTest(DepthTest depthTest);

	/**
	* Sets the time step
	* @param timeStep The time step
	*/
	void SetTimeStep(float timeStep);

	/**
	* @returns The current time step
	*/
	float GetTimeStep() const;

	/**
	* Sets the time
	* @param time The time
	*/
	void SetTime(float time);

	/**
	* @returns The current time
	*/
	float GetTime() const;

	/**
	* @returns A render surface to be used as the target for shadow maps
	*/
	RenderSurface *GetShadowMap();

	/**
	* Sets a vertex buffer to be used for rendering
	*/
	void SetBuffer(VertexBuffer *vertexBuffer);

	/**
	* Sets a scene view for rendering. The views are rendered in reverse order
	* @param index The index of the scene view
	* @param sceneView The scene view to add
	*/
	void SetSceneView(unsigned int index, SceneView *sceneView);

	/**
	* Renders all scene views
	*/
	void Render();

	/**
	* Renders a single scene view
	*/
	void RenderSceneView(SceneView *sceneView);

	void RemoveUnusedRenderSurfaces();

	void GetRenderTargetSize(int &width, int &height);

private:

	const float MAX_UNUSED_RENDERSURFACE_DURATION = 1.0f;

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
	SharedPtr<RenderSurface> shadowMap;
	bool fboDirty;	
	SharedPtr<RenderPath> defaultRenderPath;
	int width, height;
	BlendMode blendMode;
	bool depthWrite;
	DepthTest depthTest;
	float timeStep;
	float time;
	std::vector<SharedPtr<SceneView>> sceneViews;
};

}
#pragma warning(pop)
