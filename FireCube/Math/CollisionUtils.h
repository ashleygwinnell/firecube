#pragma once

#include <vector>
#include "Math/Math.h"

namespace FireCube
{

class CollisionMesh;
class CollisionTriangle;
class CollisionResult;
class Plane;
class BoundingBox;

namespace CollisionUtils
{

void SweepEllipsoidMesh(vec3 transformedPosition, vec3 transformedVelocity, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result);
void SweepSphereMesh(vec3 position, vec3 velocity, float radius, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result);
bool SweepSphereTriangle(vec3 position, vec3 velocity, float radius, const CollisionTriangle &tri, float &intersectionDistance, vec3 &intersectionPoint, vec3 &intersectionNormal);
bool SweepCapsuleMesh(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const CollisionMesh &collisionMesh, mat4 transform, CollisionResult &result);
bool SweepCapsulePlane(vec3 dir, float distance, vec3 capsuleP0, vec3 capsuleP1, float radius, const Plane &plane, mat4 transform, CollisionResult &result);
bool SweepSpherePlane(vec3 position, vec3 velocity, float radius, const Plane &plane, mat4 transform, CollisionResult &result);
int IntersectRayCapsule(vec3 origin, vec3 dir, vec3 capsuleP0, vec3 capsuleP1, float radius, float t[2]);
bool IntersectRaySphere(const vec3 &origin, const vec3 &dir, float length, const vec3& center, float radius, float& dist);
bool SweepSphereCapsule(const vec3& sphereCenter, float sphereRadius, const vec3& capusleP0, const vec3& capusleP1, float radius, const vec3& dir, float length, CollisionResult &result);
bool IntersectOBBOBB(const BoundingBox &box0, mat4 &box0Transform, const BoundingBox &box1, mat4 &box1Transform);
bool IntersectOBBOBB(const vec3 &box0Center, const vec3 box0Axes[3], const vec3 &box0Size, const vec3 &box1Center, const vec3 box1Axes[3], const vec3 &box1Size);
vec3 ClosestPoint(const BoundingBox &box0, const mat4 &box0Transform, const vec3 &point);
bool IntersectOBBSphere(const BoundingBox &box, const mat4 &boxTransform, const vec3 &sphereCenter, float radius);
bool IntersectSphereSphere(const vec3 center0, float radius0, const vec3 &center1, float radius1);
}

}