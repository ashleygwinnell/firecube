#include <algorithm>
#include <set>

#include "Physics/PhysicsWorld.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"
#include "Physics/RigidBody.h"
#include "Core/Events.h"
#include "Scene/Node.h"

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

void PhysicsWorld::UpdateCharacterControllers(float deltaTime)
{	
	for (auto characterController : characterControllers)
	{
		characterController->UpdateTransformedState();

		std::set<CollisionShape *> triggeredCollisionShapes;
		for (int iter = 0; iter < 3; ++iter)
		{
			if (characterController->finishedMovement == false && (characterController->transformedVelocity * characterController->radius).Length2() > 0.001f * 0.001f)
			{				
				characterController->collisionFound = false;
				BoundingBox characterControllerBoundingBox = BoundingBox(characterController->transformedPosition * characterController->GetRadius() - characterController->GetRadius(), characterController->transformedPosition * characterController->GetRadius() + characterController->GetRadius());
				characterControllerBoundingBox.Expand(characterControllerBoundingBox.GetMin() + characterController->transformedVelocity * characterController->GetRadius());
				characterControllerBoundingBox.Expand(characterControllerBoundingBox.GetMin() - characterController->transformedVelocity * characterController->GetRadius());
				characterControllerBoundingBox.Expand(characterControllerBoundingBox.GetMax() + characterController->transformedVelocity * characterController->GetRadius());
				characterControllerBoundingBox.Expand(characterControllerBoundingBox.GetMax() - characterController->transformedVelocity * characterController->GetRadius());

				std::vector<CollisionShape *> closeCollisionShapes;				
				octree.GetObjects(characterControllerBoundingBox, closeCollisionShapes);

				for (auto collisionShape : closeCollisionShapes)
				{
					if ((collisionShape->IsTrigger() && triggeredCollisionShapes.find(collisionShape) == triggeredCollisionShapes.end()) || characterControllerBoundingBox.Intersects(collisionShape->GetWorldBoundingBox()))
					{
						CollisionResult result;

						if (collisionShape->GetShapeType() == CollisionShapeType::TRIANGLE_MESH || collisionShape->GetShapeType() == CollisionShapeType::BOX)
						{
							CollisionMesh *mesh = collisionShape->GetCollisionMesh();
							characterController->CheckCollisionWithMesh(*mesh, collisionShape->GetNode()->GetWorldTransformation(), result);
						}
						else if (collisionShape->GetShapeType() == CollisionShapeType::PLANE)
						{
							characterController->CheckCollisionWithPlane(collisionShape->GetPlane(), collisionShape->GetNode()->GetWorldTransformation(), result);
						}

						if (collisionShape->IsTrigger() == false)
						{
							characterController->contacts.insert(characterController->contacts.end(), result.contacts.begin(), result.contacts.end());
							if (result.collisionFound && ((characterController->collisionFound == false) || (result.nearestDistance < characterController->nearestDistance)))
							{
								characterController->nearestTime = result.nearestTime;
								characterController->nearestDistance = result.nearestDistance;
								characterController->nearestIntersectionPoint = result.nearestIntersectionPoint;
								characterController->nearestNormal = result.nearestNormal;
								characterController->collisionFound = true;
							}
						}
						else if (result.collisionFound)
						{
							triggeredCollisionShapes.insert(collisionShape);
						}
					}
				}

				if (characterController->collisionFound == false)
				{
					characterController->transformedPosition += characterController->transformedVelocity;
					characterController->finishedMovement = true;
				}
				else
				{
					characterController->transformedPosition += (characterController->transformedVelocity.Normalized() * (characterController->nearestDistance - 0.001f));
					vec3 normal = characterController->nearestNormal.Normalized();
					characterController->transformedVelocity -= normal * normal.Dot(characterController->transformedVelocity);
				}
			}
		}

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
		if (characterController->onGround || characterController->collisionFound == false)
		{
			characterController->velocity.x *= 0.7f;
			characterController->velocity.z *= 0.7f;
		}
		characterController->velocity += vec3(0, -0.2f, 0) * deltaTime;
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