#pragma once

#include "Core/Component.h"
#include "Math/Math.h"
#include "Math/Plane.h"
#include "Physics/PhysicsWorld.h"

namespace FireCube
{

class PhysicsWorld;
class CollisionMesh;

class CollisionResult;

/**
* This class represnets a capsule character controller component
*/
class CharacterController : public Component
{
	friend PhysicsWorld;
	FIRECUBE_OBJECT(CharacterController)
public:
	CharacterController(Engine *engine);
	~CharacterController();
	
	/**
	* Sets the radius of the character controller
	* @param radius The radius
	*/
	void SetRadius(float radius);
	
	/**
	* @returns The radius of the character controller
	*/
	float GetRadius() const;
	
	/**
	* Sets the height of the character controller
	* @param height The height
	*/
	void SetHeight(float height);
	
	/**
	* @returns The height of the character controller
	*/
	float GetHeight() const;
	
	/**
	* Sets the velocity of the character controller
	* @param velocity The velocity
	*/
	void SetVelocity(vec3 velocity);
	
	/**
	* @returns The velocity of the character controller
	*/
	vec3 GetVelocity() const;
	
	/**
	* @returns Whether the character controller is standing on the ground
	*/
	bool IsOnGround() const;
	
	/**
	* Sets the contact offset of the character controller. The contact offset is a small margin around the capsule to avoid numerical issues which might happen when
	* the controller touches other objects (this offset effectively expands the capsule)
	* @param contactOffset The offset to set
	*/
	void SetContactOffset(float contactOffset);
	
	/**
	* @returns The contact offset of this character controller
	*/
	float GetContactOffset() const;
	
	/**
	* Sets the step height for auto-stepping. The step height determines the height of obstacles from the bottom of the cylinder which the character controller automatically climbs over
	*/
	void SetStepOffset(float stepOffset);
	
	/**
	* @returns The step offset
	*/
	float GetStepOffset() const;

	/**
	* Renders debug geometry of the character controller
	* @param debugRenderer The debug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	/**
	* Clones this character controller  
	*/
	virtual Component *Clone() const;
private:
	
	CharacterController(const CharacterController &other);

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	
	vec3 position;
	vec3 velocity;
	float radius;
	float height;
	float contactOffset;
	float stepOffset;
	
	std::vector<CollisionContact> contacts;
	bool onGround;	
	WeakPtr<PhysicsWorld> physicsWorld;
};

}