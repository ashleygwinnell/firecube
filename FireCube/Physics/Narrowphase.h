#pragma once

#include <vector>

#include "Math/Math.h"

namespace FireCube
{
	
class RigidBody;
class Equation;
class PhysicsWorld;
class CollisionShape;
class ContactConstraint;
class FrictionConstraint;

class Narrowphase
{
public:
	Narrowphase(PhysicsWorld *physicsWorld);

	void GetContacts(std::vector<RigidBody *> &p1, std::vector<RigidBody *> &p2);
	std::vector<ContactConstraint *> &GetContactConstraints();
	std::vector<FrictionConstraint *> &GetFrictionConstraints();
private:

	void SpherePlane(CollisionShape *si, CollisionShape *sj, vec3 xi, vec3 xj, quat qi, quat qj, RigidBody *bi, RigidBody *bj);
	void SphereSphere(CollisionShape *si, CollisionShape *sj, vec3 xi, vec3 xj, quat qi, quat qj, RigidBody *bi, RigidBody *bj);

	ContactConstraint *CreateContactConstraint(RigidBody *bi, RigidBody *bj);
	bool CreateFrictionConstraintsFromContact(ContactConstraint *contactConstraint, std::vector<FrictionConstraint *> &frictionConstraints);

	PhysicsWorld *physicsWorld;

	std::vector<ContactConstraint *> contactConstraints;
	std::vector<FrictionConstraint *> frictionConstraints;
};

}

