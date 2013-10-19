#pragma once

#include <memory>
#include "Utils/utils.h"
#include "Math/Math.h"
#include "Math/BoundingBox.h"

namespace FireCube
{

class VertexBuffer;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
class Engine;
class Geometry;
typedef std::shared_ptr<Geometry> GeometryPtr;
class Camera;

class FIRECUBE_API DebugRenderer
{
public:
	DebugRenderer(Engine *engine);
	void Initialize();
	void Render(Camera *camera);
	void AddLine(const vec3 &start, const vec3 &end, const vec3 &color);
	void AddBoundingBox(const BoundingBox &boundingBox, const vec3 &color);	
private:
	VertexBufferPtr linesVertexBuffer;
	GeometryPtr linesGeometry;
	std::vector<vec3> lines;
	Engine *engine;
};

}