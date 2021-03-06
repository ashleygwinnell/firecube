#pragma once

#include <set>
#include <unordered_map>

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Octree.h"
#include "Physics/CollisionShape.h"
#include "Physics/RigidBody.h"
#include "Physics/Narrowphase.h"
#include "Physics/Solver.h"

namespace FireCube
{

class CharacterController;
class PhysicsRayQuery;

class CollisionContact
{
public:	
	float time;
	float distance;
	vec3 normal;
	vec3 intersectionPoint;
};

class CollisionResult
{
public:
	CollisionResult();

	bool collisionFound;	
	float nearestDistance;
	vec3 nearestNormal;
	vec3 nearestIntersectionPoint;
	vec3 nearestTriNormal;	
};

struct SimpleHash {
	size_t operator()(const std::pair<CollisionShape *, CollisionShape *> &p) const
	{		
		return ((size_t) p.first) ^ ((size_t) p.second);
	}
};

class FIRECUBE_API PhysicsWorld : public Component
{
	FIRECUBE_OBJECT(PhysicsWorld)
public:
	PhysicsWorld(Engine *engine);
	void AddCollisionShape(CollisionShape *collisionShape);
	void RemoveCollisionShape(CollisionShape *collisionShape);
	
	void AddCharacterController(CharacterController *characterController);
	void RemoveCharacterController(CharacterController *characterController);

	void AddRigidBody(RigidBody *rigidBody);
	void RemoveRigidBody(RigidBody *rigidBody);
	std::vector<RigidBody *> &GetRigitBodies();

	void SetGravity(vec3 gravity);
	vec3 GetGravity() const;

	float GetDeltaTime() const;

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);

	void IntersectRay(PhysicsRayQuery &rayQuery);

	virtual Component *Clone() const;
private:

	PhysicsWorld(const PhysicsWorld &other);

	void Update(float deltaTime);
	void UpdateCharacterControllers(float deltaTime);
	bool SweepCharacterController(CharacterController *characterController, vec3 velocity, std::set<CollisionShape *> &triggeredCollisionShapes, unsigned int maxIterations, bool &collided);
	static void CollideCharacterController(const CharacterController *characterController, vec3 direction, float distance, const BoundingBox &characterControllerBoundingBox, std::vector<CollisionShape *> &collisionShapes, std::set<CollisionShape *> &triggeredCollisionShapes, CollisionResult &result);
	void UpdateCharacterController(CharacterController *characterController, float deltaTime);
	void UpdateRigidBodies(float deltaTime);
	void MarkedDirty();
	void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	void CheckCollisionShapesCollisions();

	std::vector<CollisionShape *> collisionShapes;
	std::vector<CharacterController *> characterControllers;
	std::vector<RigidBody *> rigidBodies;

	vec3 gravity;
	float deltaTime;
	Narrowphase narrowphase;
	Solver solver;

	Octree<CollisionShape> collisionShapesOctree;
	Octree<RigidBody> rigidBodiesOctree;
	std::unordered_map<std::pair<CollisionShape *, CollisionShape *>, bool, SimpleHash> collidingShapes;
};

}