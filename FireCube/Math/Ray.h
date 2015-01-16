#pragma once

#include "Utils/utils.h"
#include "Math/Math.h"
#include "Math/BoundingBox.h"
#include "Math/Plane.h"

namespace FireCube
{

class FIRECUBE_API Ray
{
public:
	Ray();
	Ray(const vec3 &origin, const vec3 &direction);
	bool IntersectTriangle(const vec3 &vert0, const vec3 &vert1, const vec3 &vert2, float &u, float &v, float &t) const;
	bool IntersectBoundingBox(const BoundingBox &boundingBox, float &distance) const;
	bool IntersectMesh(const char *vertexData, unsigned int vertexSize, const char *indexData, unsigned int indicesCount, float &distance, vec3 &normal) const;
	bool IntersectMesh(const char *vertexData, unsigned int vertexCount, unsigned int vertexSize, float &distance, vec3 &normal) const;
	Ray Transformed(const mat4 &transformation) const;
	bool Inside(const BoundingBox &boundingBox) const;
	bool IntersectPlane(const Plane &plane, float &distance) const;
	vec3 origin;
	vec3 direction;
};

}