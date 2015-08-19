#pragma once

#include <vector>
#include "Math/Math.h"

namespace FireCube
{

class CollisionMesh;
class CollisionTriangle;
class CollisionResult;

namespace CollisionUtils
{

void SweepEllipsoidMesh(vec3 transformedPosition, vec3 transformedVelocity, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result);
void SweepSphereMesh(vec3 position, vec3 velocity, float radius, const CollisionMesh &collisionMesh, CollisionResult &result);
bool SweepSphereTriangle(vec3 position, vec3 velocity, float radius, const CollisionTriangle &tri, float &intersectionDistance, vec3 &intersectionPoint, vec3 &intersectionNormal);
bool SweepCapsuleMesh(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const CollisionMesh &collisionMesh, CollisionResult &result);

}

}