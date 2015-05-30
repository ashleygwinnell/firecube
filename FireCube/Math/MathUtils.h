#pragma once

#include <vector>
#include "Math/Math.h"

namespace FireCube
{

class CollisionMesh;
class CollisionResult;

namespace MathUtils
{
	
unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );
void CalculateNormals(std::vector<vec3> &normals, const std::vector<vec3> &vertices, const std::vector<unsigned int> &indices);
void CalculateTangents(const std::vector<vec3> &vertices, const std::vector<vec3> &normals, const std::vector<vec2> &uv, const std::vector<unsigned int> &indices, std::vector<vec3> &tangents);
bool PointTri(vec3 aa, vec3 bb, vec3 cc, vec3 p);
bool GetLowestRoot(float a, float b, float c, float maxR, float &root);
void SweepEllipsoidMesh(vec3 transformedPosition, vec3 transformedVelocity, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result);
}

}