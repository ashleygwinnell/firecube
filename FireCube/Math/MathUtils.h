#pragma once

#include <vector>
#include "Math/Math.h"

#define EPSILON 0.00001f

namespace FireCube
{

class CollisionMesh;
class CollisionTriangle;
class CollisionResult;
class Plane;

namespace MathUtils
{
	
unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );
void CalculateNormals(std::vector<vec3> &normals, const std::vector<vec3> &vertices, const std::vector<unsigned int> &indices);
void CalculateTangents(const std::vector<vec3> &vertices, const std::vector<vec3> &normals, const std::vector<vec2> &uv, const std::vector<unsigned int> &indices, std::vector<vec3> &tangents);
bool PointTri(vec3 aa, vec3 bb, vec3 cc, vec3 p);
bool GetLowestRoot(float a, float b, float c, float maxR, float &root);
vec3 ClosestPointTriangle(const vec3 &p, const vec3 &a, const vec3 &b, const vec3 &c, float& s, float& t);
vec3 LineSegmentTriangleClosestPoint(const vec3 &s0, const vec3 &s1, const vec3 &p0, const vec3 &p1, const vec3 &p2, vec3 &pointOnSegment);
bool IntersectRayTriangle(const vec3 &origin, const vec3 &direction, const vec3 &vert0, const vec3 &vert1, const vec3 &vert2, float &u, float &v, float &t);
vec3 GetTrianglePoint(const vec3 &p0, const vec3 &p1, const vec3 &p2, float u, float v);
vec3 ClosestPointToTriangleEdge(const vec3 &s0, const vec3 &s1, const vec3 &p0, const vec3 &p1, const vec3 &p2, float &outU, float &outV, float &outD);
void ClosestPointLineLine(const vec3 &v0, const vec3 &v10, const vec3 &v2, const vec3 &v32, float &d, float &d2);
vec3 ClosestPointLineSegmentLineSegment(const vec3 &s0, const vec3 &s1, const vec3 &l0, const vec3 &l1, float &d, float &d2);
vec3 ClosestPointLineSegment(const vec3 &point, const vec3 &s, const vec3 &s1, float &d);
bool Intersect3Planes(const Plane &plane1, const Plane &plane2, const Plane &plane3, vec3 &intersectionPoint);
}

}