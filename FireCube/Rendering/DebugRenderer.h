#pragma once

#include <memory>
#include "Utils/utils.h"
#include "Math/Math.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

class VertexBuffer;
class Engine;
class Geometry;
class Camera;
class Frustum;

/**
* A class used to render debug geometry (lines)
*/
class FIRECUBE_API DebugRenderer
{
public:
	DebugRenderer(Engine *engine);
	~DebugRenderer();
	
	/**
	* Initializes the debug renderer
	*/
	void Initialize();
	
	/**
	* Renders all the lines added to the renderer and then removes them
	* @param camera Teh camera to use for rendering
	*/
	void Render(Camera *camera);
	
	/**
	* Adds a single lines
	* @param start The start position of the line
	* @param end The end position of the line
	* @param color The color of the line
	*/
	void AddLine(const vec3 &start, const vec3 &end, const vec3 &color);
	
	/**
	* Adds an axis aligned bounding box
	* @color The color of the box
	*/
	void AddBoundingBox(const BoundingBox &boundingBox, const vec3 &color);
	
	/**
	* Adds a sphere
	* @param center The center of the sphere
	* @param radius The radius of the sphere
	* @param rings Vertical tesselation of the sphere
	* @param columns Horizontal tesselation of the sphere
	* @param color The color of the sphere
	*/	
	void AddSphere(vec3 center, float radius, unsigned int rings, unsigned int columns, const vec3 &color);
	
	/**
	* Adds a capsule
	* @param p0 The start position of the segment of the capsule 
	* @param p1 The end position of the segment of the capsule
	* @param radius The radius of the capsule
	* @param rings Vertical tesselation of the capsule
	* @param columns Horizontal tesselation of the capsule
	* @param color The color of the capsule
	*/
	void AddCapsule(vec3 p0, vec3 p1, float radius, unsigned int rings, unsigned int columns, const vec3 &color);
	
	/**
	* Adds a frustum
	* @param frustum the frustum
	* @param color The color of the frustum 
	*/
	void AddFrustum(const Frustum &frustum, const vec3 &color);
	
	/**
	* Adds a frustum
	* @param viewMatrix The view matrix to use to extract the frustum from
	* @param projectionMatrix The projection matrix to use to extract the frustum from
	* @param color The color of the frustum 
	*/
	void AddFrustum(const mat4 &viewMatrix, const mat4 &projectionMatrix, const vec3 &color);
private:
	struct LineVertex
	{
		vec3 position;
		vec4 color;
	};
	VertexBuffer *linesVertexBuffer;
	Geometry *linesGeometry;
	std::vector<LineVertex> lines;
	Engine *engine;
};

}