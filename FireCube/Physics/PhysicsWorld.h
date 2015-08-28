#pragma once

#include <set>

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Octree.h"
#include "Physics/CollisionShape.h"

namespace FireCube
{

class CharacterController;
class RigidBody;

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

class FIRECUBE_API PhysicsWorld : public Component
{
	OBJECT(PhysicsWorld)
public:
	PhysicsWorld(Engine *engine);
	void AddCollisionShape(CollisionShape *collisionShape);
	void RemoveCollisionShape(CollisionShape *collisionShape);
	
	void AddCharacterController(CharacterController *characterController);
	void RemoveCharacterController(CharacterController *characterController);

	void AddRigidBody(RigidBody *rigidBody);
	void RemoveRigidBody(RigidBody *rigidBody);

	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
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

	std::vector<CollisionShape *> collisionShapes;
	std::vector<CharacterController *> characterControllers;
	std::vector<RigidBody *> rigidBodies;

	Octree<CollisionShape> octree;
};

}