#include <algorithm>
#include "Physics/PhysicsWorld.h"
#include "Physics/CharacterController.h"
#include "Physics/CollisionShape.h"
#include "Physics/RigidBody.h"
#include "Core/Events.h"
#include "Scene/Node.h"
#include "Math/CollisionUtils.h"
#include "Physics/PhysicsCollisionQuery.h"

using namespace FireCube;

PhysicsWorld::PhysicsWorld(Engine *engine) : Component(engine), collisionShapesOctree(engine, 100.0f, 8, 1.0f), rigidBodiesOctree(engine, 100.0f, 8, 1.0f), narrowphase(this)
{
	SubscribeToEvent(Events::Update, &PhysicsWorld::Update);
	solver.SetSpookParams(1e7f, 3, 1.0f / 60.0f);
}

PhysicsWorld::PhysicsWorld(const PhysicsWorld &other) : Component(other), narrowphase(this), collisionShapes(other.collisionShapes), characterControllers(other.characterControllers), rigidBodies(other.rigidBodies), collisionShapesOctree(engine, 100.0f, 8, 1.0f), rigidBodiesOctree(engine, 100.0f, 8, 1.0f)
{
	SubscribeToEvent(Events::Update, &PhysicsWorld::Update);
	solver.SetSpookParams(1e7f, 3, 1.0f / 60.0f);
}

void PhysicsWorld::AddCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.push_back(collisionShape);
	collisionShape->SetOctree(&collisionShapesOctree);
	if (collisionShape->GetWorldBoundingBox().Valid())
	{
		collisionShapesOctree.Insert(collisionShape);
	}
}

void PhysicsWorld::RemoveCollisionShape(CollisionShape *collisionShape)
{
	collisionShapes.erase(std::remove(collisionShapes.begin(), collisionShapes.end(), collisionShape), collisionShapes.end());	

	collisionShape->SetOctreeNodeNeedsUpdate(false);
	if (collisionShape->GetOctreeNode())
	{
		collisionShapesOctree.Remove(collisionShape);
		collisionShape->SetOctree(nullptr);
	}

	if (collisionShape->IsTrigger())
	{
		std::unordered_map<std::pair<CollisionShape *, CollisionShape *>, bool, SimpleHash> newCollidingShapes;

		for (auto item : collidingShapes)
		{
			if (item.first.first == collisionShape || item.first.second == collisionShape)
			{
				Events::CollisionShapeLeaveCollision(item.first.first, item.first.first, item.first.second);
			}
			else
			{
				newCollidingShapes[item.first] = item.second;
			}
		}

		collidingShapes = newCollidingShapes;
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
	rigidBody->SetOctree(&rigidBodiesOctree);
	if (rigidBody->GetWorldBoundingBox().Valid())
	{
		rigidBodiesOctree.Insert(rigidBody);
	}
}

void PhysicsWorld::RemoveRigidBody(RigidBody *rigidBody)
{
	rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), rigidBody), rigidBodies.end());

	rigidBody->SetOctreeNodeNeedsUpdate(false);
	if (rigidBody->GetOctreeNode())
	{
		rigidBodiesOctree.Remove(rigidBody);
		rigidBody->SetOctree(nullptr);
	}
}

void PhysicsWorld::CollideCharacterController(const CharacterController *characterController, vec3 direction, float distance, const BoundingBox &characterControllerBoundingBox, std::vector<CollisionShape *> &collisionShapes, std::set<CollisionShape *> &triggeredCollisionShapes, CollisionResult &result)
{
	vec3 p0 = characterController->position + vec3(0, characterController->height * 0.5f, 0);
	vec3 p1 = characterController->position - vec3(0, characterController->height * 0.5f, 0);

	for (auto collisionShape : collisionShapes)
	{
		if (collisionShape->IsOwnedByRigidBody() == false && characterControllerBoundingBox.Intersects(collisionShape->GetWorldBoundingBox()) &&
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
				CollisionUtils::SweepCapsulePlane(direction, distance, p0, p1, characterController->radius, collisionShape->GetPlane(), collisionShape->GetNode()->GetWorldTransformation(), currentResult);
			}
			else if (collisionShape->GetShapeType() == CollisionShapeType::SPHERE)
			{
				CollisionUtils::SweepSphereCapsule(collisionShape->GetNode()->GetWorldPosition(), collisionShape->GetRadius(), p0, p1, characterController->radius, -direction, distance, currentResult);				
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

bool PhysicsWorld::SweepCharacterController(CharacterController *characterController, vec3 velocity, std::set<CollisionShape *> &triggeredCollisionShapes, unsigned int maxIterations, bool &collided)
{
	collided = false;
	bool moved = false;
	if (velocity.Length2() < 0.001f * 0.001f)
	{
		return false;
	}

	vec3 originalVelocity = velocity;
	
	for (unsigned int iter = 0; iter < maxIterations; ++iter)
	{
		if (velocity.Length2() > 0.001f * 0.001f)
		{
			// If going in the other direction of the original direction, stop to prevent shakings in corners
			if ((velocity.Dot(originalVelocity)) <= 0.0f)
				break;

			vec3 characterControllerExtents(characterController->GetRadius() + characterController->GetContactOffset(), characterController->GetHeight() * 0.5f + characterController->GetRadius() + characterController->GetContactOffset(), characterController->GetRadius() + characterController->GetContactOffset());
			BoundingBox characterControllerBoundingBox = BoundingBox(characterController->position - characterControllerExtents, characterController->position + characterControllerExtents);
			BoundingBox characterControllerBoundingBoxAtTarget = BoundingBox(characterController->position + velocity - characterControllerExtents, characterController->position + velocity + characterControllerExtents);
			characterControllerBoundingBox.Expand(characterControllerBoundingBoxAtTarget);

			std::vector<CollisionShape *> closeCollisionShapes;
			collisionShapesOctree.GetObjects(characterControllerBoundingBox, closeCollisionShapes);
			CollisionResult result;
			CollideCharacterController(characterController, velocity.Normalized(), velocity.Length() + characterController->GetContactOffset(), characterControllerBoundingBox, closeCollisionShapes, triggeredCollisionShapes, result);
			
			moved = true;
			if (result.collisionFound == false)
			{
				characterController->position += velocity;
				break;
			}
			else
			{				
				collided = true;
				if (result.nearestDistance > characterController->contactOffset)
				{
					characterController->position += (velocity.Normalized() * (result.nearestDistance - characterController->contactOffset));
				}
				vec3 normal = result.nearestNormal.Normalized();
				velocity -= normal * normal.Dot(velocity);
			}
		}
	}

	return moved;
}

void DecomposeVector(vec3 dir, vec3 up, vec3 &tangentComponent, vec3 &normalComponent)
{
	normalComponent = dir.Dot(up) * up;
	tangentComponent = dir - normalComponent;
}

void PhysicsWorld::UpdateCharacterController(CharacterController *characterController, float deltaTime)
{
	characterController->position = characterController->GetNode()->GetWorldPosition();
	characterController->contacts.clear();
	
	characterController->velocity += vec3(0, -0.5f, 0) * deltaTime;

	std::set<CollisionShape *> triggeredCollisionShapes;
	unsigned int maxIterations = 3;
	float stepOffset = characterController->stepOffset;
	const vec3 up(0, 1, 0);
	vec3 upDirection = vec3::ZERO, sideDirection = vec3::ZERO, downDirection = vec3::ZERO, normalComponent;
	float velocityDotUp = characterController->velocity.Dot(up);
	DecomposeVector(characterController->velocity, up, sideDirection, normalComponent);
	if (velocityDotUp <= 0.0f)
		downDirection = normalComponent;
	else
		upDirection = normalComponent;

	bool haveSideMovement = sideDirection.Length2() > 0.001f * 0.001f;
	if (haveSideMovement)
	{
		upDirection += up * stepOffset;
	}

	float startHeight = characterController->position.Dot(up);

	bool collided;
	if (SweepCharacterController(characterController, upDirection, triggeredCollisionShapes, maxIterations, collided))
	{
		float heightDiff = characterController->position.Dot(up) - startHeight;
		if (heightDiff < stepOffset)
		{
			stepOffset = heightDiff;
		}

	}

	SweepCharacterController(characterController, sideDirection, triggeredCollisionShapes, maxIterations, collided);

	if (haveSideMovement)
	{
		downDirection -= up * stepOffset;
	}

	characterController->onGround = false;

	if (SweepCharacterController(characterController, downDirection, triggeredCollisionShapes, 1, collided))
	{
		if (collided)
		{			
			characterController->velocity.y = 0;
			characterController->onGround = true;
		}
	}

	characterController->velocity.x *= 0.7f;
	characterController->velocity.z *= 0.7f;
	

	for (auto triggeredCollisionShape : triggeredCollisionShapes)
	{
		Events::CharacterControllerCollision(characterController, characterController, triggeredCollisionShape);
	}	

	characterController->GetNode()->SetTranslation(characterController->position);
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
		rigidBody->SetPosition(rigidBody->GetNode()->GetWorldPosition());
		rigidBody->SetRotation(rigidBody->GetNode()->GetWorldRotation());
		if (rigidBody->GetBodyType() == RigidBodyType::DYNAMIC)
		{
			rigidBody->force += gravity * rigidBody->mass;
		}
	}

	std::vector<RigidBody *> p1;
	std::vector<RigidBody *> p2;
	for (auto rigidBody : rigidBodies)
	{
		std::vector<RigidBody *> closeRigidBodies;
		rigidBodiesOctree.GetObjects(rigidBody->GetWorldBoundingBox(), closeRigidBodies);
		for (auto &other : closeRigidBodies)
		{
			if (rigidBody < other)
			{
				p1.push_back(rigidBody);
				p2.push_back(other);
			}

		}		
	}

	narrowphase.GetContacts(p1, p2);

	auto contactConstraints = narrowphase.GetContactConstraints();
	auto frictionConstraints = narrowphase.GetFrictionConstraints();	
	
	for (auto c : frictionConstraints)
	{
		solver.AddConstraint((Constraint *) c);
	}

	for (auto c : contactConstraints)
	{
		solver.AddConstraint((Constraint *) c);
	}

	solver.Solve(deltaTime, this);
	solver.Clear();

	for (auto rigidBody : rigidBodies)
	{
		// Apply damping
		if (rigidBody->GetBodyType() == RigidBodyType::DYNAMIC)
		{
			float linearDamping = 0.01f; // TODO: Set as body property
			float angularDamping = 0.01f; // TODO: Set as body property

			float ld = std::pow(1.0f - linearDamping, deltaTime);
			rigidBody->SetVelocity(rigidBody->GetVelocity() * ld);

			float ad = std::pow(1.0f - angularDamping, deltaTime);
			rigidBody->SetAngularVelocity(rigidBody->GetAngularVelocity() * ad);
		}

		rigidBody->Integrate(deltaTime);
		rigidBody->GetNode()->SetRotation(rigidBody->GetRotation());
		rigidBody->GetNode()->SetTranslation(rigidBody->GetPosition());		

		// Clear forces
		rigidBody->SetForce(vec3::ZERO);
		rigidBody->SetTorque(vec3::ZERO);
	}
}

void PhysicsWorld::Update(float deltaTime)
{	
	this->deltaTime = deltaTime;
	solver.SetSpookParams(1e7f, 3, deltaTime); // TODO: Switch to fixed time step
	collisionShapesOctree.Update();
	rigidBodiesOctree.Update();
	UpdateCharacterControllers(deltaTime);	
	UpdateRigidBodies(this->deltaTime);
	CheckCollisionShapesCollisions();
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
	collisionShapesOctree.RenderDebugGeometry(debugRenderer);

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

std::vector<RigidBody *> &PhysicsWorld::GetRigitBodies()
{
	return rigidBodies;
}

void PhysicsWorld::SetGravity(vec3 gravity)
{
	this->gravity = gravity;
}

vec3 PhysicsWorld::GetGravity() const
{
	return gravity;
}

float PhysicsWorld::GetDeltaTime() const
{
	return deltaTime;
}

void PhysicsWorld::IntersectRay(PhysicsRayQuery &rayQuery)
{
	std::vector<CollisionShape *> collisionShapes;

	collisionShapesOctree.GetObjects(rayQuery.ray, rayQuery.maxDistance, collisionShapes);
	for (auto shape : collisionShapes)
	{
		mat4 worldTransform = shape->GetNode()->GetWorldTransformation();
		mat4 modelTransform = worldTransform;
		modelTransform.Inverse();
		Ray localRay = rayQuery.ray.Transformed(modelTransform);

		if (shape->GetShapeType() == CollisionShapeType::BOX)
		{
			float dist;
			vec3 normal;
			if (localRay.IntersectBoundingBox(shape->GetBox(), dist, normal) && dist <= rayQuery.maxDistance)
			{
				PhysicsRayQueryResult result;
				result.distance = dist;
				result.shape = shape;
				result.normal = normal.TransformNormal(worldTransform);
				rayQuery.results.push_back(result);
			}
		}
		else if (shape->GetShapeType() == CollisionShapeType::SPHERE)
		{
			float dist;
			if (CollisionUtils::IntersectRaySphere(localRay.origin, localRay.direction, rayQuery.maxDistance, vec3(0.0f), shape->GetRadius(), dist) && dist <= rayQuery.maxDistance)
			{
				PhysicsRayQueryResult result;
				result.distance = dist;
				result.shape = shape;
				result.normal = localRay.origin.Normalized().TransformNormal(worldTransform);
				rayQuery.results.push_back(result);
			}
		}
		else if (shape->GetShapeType() == CollisionShapeType::PLANE)
		{
			float dist;
			if (localRay.IntersectPlane(shape->GetPlane(), dist) && dist <= rayQuery.maxDistance)
			{
				PhysicsRayQueryResult result;
				result.distance = dist;
				result.shape = shape;
				result.normal = shape->GetPlane().GetNormal().TransformNormal(worldTransform);
				rayQuery.results.push_back(result);
			}
		}
		else if (shape->GetShapeType() == CollisionShapeType::TRIANGLE_MESH)
		{
			float dist;
			vec3 normal;
			bool found = false;
			CollisionMesh *mesh = shape->GetCollisionMesh();
			for (auto &tri : mesh->triangles)
			{
				float u, v, t;
				vec3 n = Cross(tri.p1 - tri.p0, tri.p2 - tri.p0);
				if (n.Dot(localRay.direction) < 0)
				{
					bool intersect = localRay.IntersectTriangle(tri.p0, tri.p1, tri.p2, u, v, t);
					if (intersect && (t > 0) && (!found || (t < dist && t <= rayQuery.maxDistance)))
					{
						found = true;
						dist = t;
						normal = n;
					}
				}
			}

			if (found)
			{
				PhysicsRayQueryResult result;
				result.distance = dist;
				result.shape = shape;
				result.normal = normal;
				rayQuery.results.push_back(result);
			}
		}
	}
}

void PhysicsWorld::CheckCollisionShapesCollisions()
{
	std::unordered_map<std::pair<CollisionShape *, CollisionShape *>, bool, SimpleHash> newCollidingShapes;
	for (auto *shape : collisionShapes)
	{
		if (shape->IsTrigger() == false)
		{
			continue;
		}

		BoundingBox worldBoundingBox = shape->GetWorldBoundingBox();
		std::vector<CollisionShape *> potentialShapes;
		collisionShapesOctree.GetObjects(worldBoundingBox, potentialShapes);
		for (auto *otherShape : potentialShapes)
		{
			if (shape == otherShape)
			{
				continue;
			}

			if (shape->GetShapeType() == CollisionShapeType::BOX)
			{
				if (otherShape->GetShapeType() == CollisionShapeType::BOX)
				{
					if (CollisionUtils::IntersectOBBOBB(shape->GetBox(), shape->GetNode()->GetWorldTransformation(), otherShape->GetBox(), otherShape->GetNode()->GetWorldTransformation()))
					{
						auto key = std::make_pair(shape, otherShape);
						newCollidingShapes[key] = true;
						if (collidingShapes.find(key) == collidingShapes.end())
						{							
							Events::CollisionShapeEnterCollision(shape, shape, otherShape);
						}
					}
				}
				else if (otherShape->GetShapeType() == CollisionShapeType::SPHERE)
				{
					vec3 scale = otherShape->GetNode()->GetWorldScale();
					float scaledRadius = std::max(std::max(scale.x, scale.y), scale.z) * otherShape->GetRadius();
					if (CollisionUtils::IntersectOBBSphere(shape->GetBox(), shape->GetNode()->GetWorldTransformation(), otherShape->GetNode()->GetWorldPosition(), scaledRadius))
					{
						auto key = std::make_pair(shape, otherShape);
						newCollidingShapes[key] = true;
						if (collidingShapes.find(key) == collidingShapes.end())
						{
							Events::CollisionShapeEnterCollision(shape, shape, otherShape);
						}
					}
				}
			}
			else if (shape->GetShapeType() == CollisionShapeType::SPHERE)
			{
				if (otherShape->GetShapeType() == CollisionShapeType::BOX)
				{
					vec3 scale = shape->GetNode()->GetWorldScale();
					float scaledRadius = std::max(std::max(scale.x, scale.y), scale.z) * shape->GetRadius();
					if (CollisionUtils::IntersectOBBSphere(otherShape->GetBox(), otherShape->GetNode()->GetWorldTransformation(), shape->GetNode()->GetWorldPosition(), scaledRadius))
					{
						auto key = std::make_pair(shape, otherShape);
						newCollidingShapes[key] = true;
						if (collidingShapes.find(key) == collidingShapes.end())
						{
							Events::CollisionShapeEnterCollision(shape, shape, otherShape);
						}
					}
				}
				else if (otherShape->GetShapeType() == CollisionShapeType::SPHERE)
				{
					vec3 scale0 = shape->GetNode()->GetWorldScale();
					float scaledRadius0 = std::max(std::max(scale0.x, scale0.y), scale0.z) * shape->GetRadius();

					vec3 scale1 = otherShape->GetNode()->GetWorldScale();
					float scaledRadius1 = std::max(std::max(scale1.x, scale1.y), scale1.z) * otherShape->GetRadius();
					
					if (CollisionUtils::IntersectSphereSphere(shape->GetNode()->GetWorldPosition(), scaledRadius0, otherShape->GetNode()->GetWorldPosition(), scaledRadius1))
					{
						auto key = std::make_pair(shape, otherShape);
						newCollidingShapes[key] = true;
						if (collidingShapes.find(key) == collidingShapes.end())
						{
							Events::CollisionShapeEnterCollision(shape, shape, otherShape);
						}
					}
				}
			}
		}
	}

	for (auto item : collidingShapes)
	{		
		if (newCollidingShapes.find(item.first) == newCollidingShapes.end())
		{
			Events::CollisionShapeLeaveCollision(item.first.first, item.first.first, item.first.second);
		}
	}

	collidingShapes = newCollidingShapes;
}

CollisionResult::CollisionResult() : collisionFound(false)
{

}