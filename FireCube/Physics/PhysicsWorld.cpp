#include <algorithm>

#include "Physics/PhysicsWorld.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"
#include "Physics/RigidBody.h"
#include "Core/Events.h"
#include "Scene/Node.h"
#include "Math/CollisionUtils.h"

using namespace FireCube;

PhysicsWorld::PhysicsWorld(Engine *engine) : Component(engine), octree(engine, vec3(2000), 8)
{
	SubscribeToEvent(Events::Update, &PhysicsWorld::Update);
}

PhysicsWorld::PhysicsWorld(const PhysicsWorld &other) : Component(other), collisionShapes(other.collisionShapes), characterControllers(other.characterControllers), rigidBodies(other.rigidBodies), octree(engine, vec3(10000), 10)
{
	SubscribeToEvent(Events::Update, &PhysicsWorld::Update);
}

void PhysicsWorld::AddCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.push_back(collisionShape);
	octree.Insert(collisionShape);
}

void PhysicsWorld::RemoveCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.erase(std::remove(collisionShapes.begin(), collisionShapes.end(), collisionShape), collisionShapes.end());	
	if (collisionShape->GetOctreeNode())
	{
		octree.Remove(collisionShape);
	}
}

void PhysicsWorld::AddCharacterController(CharacterController *characterController)
{
	characterControllers.push_back(characterController);
}

void PhysicsWorld::RemoveCharacterController(CharacterController *characterController)
{
	characterControllers.erase(std::remove(characterControllers.begin(), characterControllers.end(), characterController), characterControllers.end());
}

void PhysicsWorld::AddRigidBody(RigidBody *rigidBody)
{
	rigidBodies.push_back(rigidBody);
}

void PhysicsWorld::RemoveRigidBody(RigidBody *rigidBody)
{
	rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), rigidBody), rigidBodies.end());
}

void PhysicsWorld::CollideCharacterController(const CharacterController *characterController, vec3 direction, float distance, const BoundingBox &characterControllerBoundingBox, std::vector<CollisionShape *> &collisionShapes, std::set<CollisionShape *> &triggeredCollisionShapes, float deltaTime, CollisionResult &result)
{
	vec3 p0 = characterController->position + vec3(0, characterController->height * 0.5f, 0);
	vec3 p1 = characterController->position - vec3(0, characterController->height * 0.5f, 0);

	for (auto collisionShape : collisionShapes)
	{
		if (characterControllerBoundingBox.Intersects(collisionShape->GetWorldBoundingBox()) &&
			(collisionShape->IsTrigger() == false || triggeredCollisionShapes.find(collisionShape) == triggeredCollisionShapes.end()))
		{
			CollisionResult currentResult;

			if (collisionShape->GetShapeType() == CollisionShapeType::TRIANGLE_MESH || collisionShape->GetShapeType() == CollisionShapeType::BOX)
			{
				CollisionMesh *mesh = collisionShape->GetCollisionMesh();				
				CollisionUtils::SweepCapsuleMesh(direction, distance, p0, p1, characterController->radius, *mesh, collisionShape->GetNode()->GetWorldTransformation(), currentResult);
			}
			else if (collisionShape->GetShapeType() == CollisionShapeType::PLANE)
			{				
				CollisionUtils::SweepCapsulePlane(direction, distance, p0, p1, characterController->radius, collisionShape->GetPlane(), collisionShape->GetNode()->GetWorldTransformation(), result);
			}

			if (currentResult.collisionFound)
			{
				if (collisionShape->IsTrigger() == false)
				{
					if (result.collisionFound == false || result.nearestDistance > currentResult.nearestDistance)
					{
						result = currentResult;
					}					
				}
				else
				{
					triggeredCollisionShapes.insert(collisionShape);
				}
			}
		}
	}
}

void PhysicsWorld::SweepCharacterController(CharacterController *characterController, vec3 velocity, std::set<CollisionShape *> &triggeredCollisionShapes, float deltaTime)
{
	if (velocity.Length2() < 0.001f * 0.001f)
	{
		return;
	}

	const unsigned maxIterations = 3;
	characterController->velocity = velocity;
	for (int iter = 0; iter < maxIterations; ++iter)
	{
		if (characterController->finishedMovement == false && characterController->velocity.Length2() > 0.001f * 0.001f)
		{
			vec3 characterControllerExtents(characterController->GetRadius() + characterController->GetContactOffset(), characterController->GetHeight() * 0.5f + characterController->GetRadius() + characterController->GetContactOffset(), characterController->GetRadius() + characterController->GetContactOffset());
			BoundingBox characterControllerBoundingBox = BoundingBox(characterController->position - characterControllerExtents, characterController->position + characterControllerExtents);
			BoundingBox characterControllerBoundingBoxAtTarget = BoundingBox(characterController->position + characterController->velocity - characterControllerExtents, characterController->position + characterController->velocity + characterControllerExtents);
			characterControllerBoundingBox.Expand(characterControllerBoundingBoxAtTarget);

			std::vector<CollisionShape *> closeCollisionShapes;
			octree.GetObjects(characterControllerBoundingBox, closeCollisionShapes);
			CollisionResult result;
			CollideCharacterController(characterController, characterController->velocity.Normalized(), characterController->velocity.Length() + characterController->GetContactOffset(), characterControllerBoundingBox, closeCollisionShapes, triggeredCollisionShapes, deltaTime, result);

			if (result.collisionFound == false)
			{
				characterController->position += characterController->velocity;
				characterController->finishedMovement = true;
			}
			else
			{				
				if (result.nearestDistance > characterController->contactOffset)
				{
					characterController->position += (characterController->velocity.Normalized() * (result.nearestDistance - characterController->contactOffset));
				}
				vec3 normal = result.nearestNormal.Normalized();
				characterController->velocity -= normal * normal.Dot(characterController->velocity);
			}
		}
	}
}

void PhysicsWorld::UpdateCharacterController(CharacterController *characterController, float deltaTime)
{
	characterController->UpdateTransformedState();

	std::set<CollisionShape *> triggeredCollisionShapes;
	SweepCharacterController(characterController, characterController->velocity, triggeredCollisionShapes, deltaTime);

	for (auto triggeredCollisionShape : triggeredCollisionShapes)
	{
		Events::CharacterControllerCollision(characterController, characterController, triggeredCollisionShape);
	}

	characterController->onGround = false;
	for (auto &contact : characterController->contacts)
	{
		if (Dot(contact.normal, vec3(0, 1, 0)) > 1.0f - 0.01f)
		{
			characterController->onGround = true;
			break;
		}
	}
	characterController->UpdateFromTransformedState();
	if (characterController->onGround || true/*characterController->collisionFound == false*/)
	{
		characterController->velocity.x *= 0.7f;
		characterController->velocity.z *= 0.7f;
	}
	characterController->velocity += vec3(0, -0.2f, 0) * deltaTime;
}

void PhysicsWorld::UpdateCharacterControllers(float deltaTime)
{	
	for (auto characterController : characterControllers)
	{
		UpdateCharacterController(characterController, deltaTime);
	}	
}

void PhysicsWorld::UpdateRigidBodies(float deltaTime)
{
	for (auto rigidBody : rigidBodies)
	{
		rigidBody->node->Move(rigidBody->velocity * deltaTime);
	}
}

void PhysicsWorld::Update(float deltaTime)
{	
	octree.Update();
	UpdateCharacterControllers(deltaTime);
	UpdateRigidBodies(deltaTime);	
}

void PhysicsWorld::MarkedDirty()
{

}

void PhysicsWorld::NodeChanged()
{

}

void PhysicsWorld::SceneChanged(Scene *oldScene)
{

}

void PhysicsWorld::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	octree.RenderDebugGeometry(debugRenderer);

	for (auto s : collisionShapes)
	{
		s->RenderDebugGeometry(debugRenderer);
	}

	for (auto c : characterControllers)
	{
		c->RenderDebugGeometry(debugRenderer);
	}
}

Component *PhysicsWorld::Clone() const
{
	PhysicsWorld *clone = new PhysicsWorld(*this);
	return clone;
}

CollisionResult::CollisionResult() : collisionFound(false)
{

}