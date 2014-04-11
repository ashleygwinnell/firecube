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

class FIRECUBE_API DebugRenderer
{
public:
	DebugRenderer(Engine *engine);
	~DebugRenderer();
	void Initialize();
	void Render(Camera *camera);
	void AddLine(const vec3 &start, const vec3 &end, const vec3 &color);
	void AddBoundingBox(const BoundingBox &boundingBox, const vec3 &color);	
private:
	VertexBuffer *linesVertexBuffer;
	Geometry *linesGeometry;
	std::vector<vec3> lines;
	Engine *engine;
};

}