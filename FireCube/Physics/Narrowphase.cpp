#include "Narrowphase.h"
#include "Physics/RigidBody.h"
#include "Physics/CollisionShape.h"
#include "Scene/Node.h"
#include "Physics/ContactConstraint.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/FrictionConstraint.h"

using namespace FireCube;

Narrowphase::Narrowphase(PhysicsWorld *physicsWorld) : physicsWorld(physicsWorld)
{

}

void Narrowphase::GetContacts(std::vector<RigidBody *> &p1, std::vector<RigidBody *> &p2)
{
	contactConstraints.clear();
	frictionConstraints.clear();

	for (unsigned int k = 0; k < p1.size(); ++k)
	{
		auto bi = p1[k];
		auto bj = p2[k];

		for (unsigned int i = 0; i < bi->GetCollisionShapes().size(); ++i)
		{
			auto si = bi->GetCollisionShapes()[i];			
			quat qi = si->GetNode()->GetWorldRotation();
			vec3 xi = si->GetNode()->GetWorldPosition();

			for (unsigned int j = 0; j < bj->GetCollisionShapes().size(); ++j)
			{
				auto sj = bj->GetCollisionShapes()[j];
				quat qj = sj->GetNode()->GetWorldRotation();
				vec3 xj = sj->GetNode()->GetWorldPosition();

				if (si->GetWorldBoundingBox().Intersects(sj->GetWorldBoundingBox()))
				{
					if (si->GetShapeType() == CollisionShapeType::SPHERE && sj->GetShapeType() == CollisionShapeType::PLANE)
					{
						SpherePlane(si, sj, xi, xj, qi, qj, bi, bj);
					}
					else if (si->GetShapeType() == CollisionShapeType::PLANE && sj->GetShapeType() == CollisionShapeType::SPHERE)
					{
						SpherePlane(sj, si, xj, xi, qj, qi, bj, bi);
					}
					if (si->GetShapeType() == CollisionShapeType::SPHERE && sj->GetShapeType() == CollisionShapeType::SPHERE)
					{
						SphereSphere(si, sj, xi, xj, qi, qj, bi, bj);
					}
				}
			}
		}
	}
}

ContactConstraint *Narrowphase::CreateContactConstraint(RigidBody *bi, RigidBody *bj)
{
	auto eq = new ContactConstraint(bi, bj);

	eq->enabled = true;

	eq->restitution = 0.0f;	

	return eq;
}

void Tangents(vec3 v, vec3 &t1, vec3 &t2)
{
	float norm = v.Length();
	if (norm > 0.0f)
	{
		float inorm = 1.0f / norm;		
		vec3 n = v * inorm;

		vec3 randVec;
		if (std::abs(n.x) < 0.9f) 
		{
			randVec.Set(1, 0, 0);
			t1 = Cross(n, randVec);
		}
		else 
		{
			randVec.Set(0, 1, 0);
			t1 =Cross(n, randVec);
		}
		t2 = Cross(n, t1);
	}
	else {
		// The normal length is zero, make something up
		t1.Set(1, 0, 0);
		t2.Set(0, 1, 0);
	}
}

bool Narrowphase::CreateFrictionConstraintsFromContact(ContactConstraint *contactConstraint, std::vector<FrictionConstraint *> &frictionConstraints)
{
	auto bi = contactConstraint->bi;
	auto bj = contactConstraint->bj;

	float friction = 0.3f;

	if (friction > 0.0f)
	{
		auto mug = friction * physicsWorld->GetGravity().Length();
		float reducedMass = (bi->GetInvMass() + bj->GetInvMass());
		if (reducedMass > 0) 
		{
			reducedMass = 1.0f / reducedMass;
		}

		FrictionConstraint *c1 = new FrictionConstraint(bi, bj, mug * reducedMass);
		FrictionConstraint *c2 = new FrictionConstraint(bi, bj, mug * reducedMass);
		
		// Copy over the relative vectors
		c1->ri = contactConstraint->ri;
		c1->rj = contactConstraint->rj;
		c2->ri = contactConstraint->ri;
		c2->rj = contactConstraint->rj;

		// Construct tangents
		Tangents(contactConstraint->ni, c1->t, c2->t);

		c1->enabled = c2->enabled = contactConstraint->enabled;

		frictionConstraints.push_back(c1);
		frictionConstraints.push_back(c2);

		return true;
	}

	return false;
}

std::vector<ContactConstraint *> &Narrowphase::GetContactConstraints()
{
	return contactConstraints;
}

std::vector<FrictionConstraint *> &Narrowphase::GetFrictionConstraints()
{
	return frictionConstraints;
}

void Narrowphase::SpherePlane(CollisionShape *si, CollisionShape *sj, vec3 xi, vec3 xj, quat qi, quat qj, RigidBody *bi, RigidBody *bj)
{
	vec3 planeNormal = (qj * sj->GetPlane().GetNormal()).Normalized();
	vec3 pointOnPlane = xj + planeNormal * sj->GetPlane().GetDistance();
	
	vec3 ni = -planeNormal;
	vec3 ri = si->GetRadius() * ni;

	vec3 pointOnPlaneToSphere = xi - pointOnPlane;
	vec3 planeToSphereOrtho = ni * Dot(ni, pointOnPlaneToSphere);
	vec3 rj = pointOnPlaneToSphere - planeToSphereOrtho;

	if (-Dot(pointOnPlaneToSphere, ni) <= si->GetRadius())
	{
		ri = ri + xi - bi->GetPosition();		
		rj = rj + pointOnPlane - bj->GetPosition();

		auto r = CreateContactConstraint(bi, bj);
		r->ri = ri;
		r->rj = rj;
		r->ni = ni;
		contactConstraints.push_back(r);
		CreateFrictionConstraintsFromContact(r, frictionConstraints);
	}
}

void Narrowphase::SphereSphere(CollisionShape *si, CollisionShape *sj, vec3 xi, vec3 xj, quat qi, quat qj, RigidBody *bi, RigidBody *bj)
{
	if ((xi - xj).Length2() <= (si->GetRadius() + sj->GetRadius()) * (si->GetRadius() + sj->GetRadius()))
	{
		// We will have only one contact in this case
		auto r = CreateContactConstraint(bi, bj);

		// Contact normal
		r->ni = (xj - xi).Normalized();
		
		// Contact point locations
		r->ri = r->ni * si->GetRadius() + xi - bi->GetPosition();
		r->rj = r->ni * -sj->GetRadius() + xj - bj->GetPosition();		

		contactConstraints.push_back(r);
		CreateFrictionConstraintsFromContact(r, frictionConstraints);
	}
}