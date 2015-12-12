#include "Physics/RigidBody.h"
#include "Physics/PhysicsWorld.h"
#include "Scene/Node.h"
#include "Math/Plane.h"
#include "Scene/Scene.h"

using namespace FireCube;

RigidBody::RigidBody(Engine *engine) : Component(engine), physicsWorld(nullptr), velocity(0.0f), force(0.0f), angularVelocity(0.0f), torque(0.0f), vlambda(0.0f), wlambda(0.0f)
{

}

RigidBody::RigidBody(const RigidBody &other) : Component(engine), velocity(other.velocity), physicsWorld(other.physicsWorld)
{

}

RigidBody::~RigidBody()
{
	if (physicsWorld)
	{
		physicsWorld->RemoveRigidBody(this);
	}
}

void RigidBody::MarkedDirty()
{

}

void RigidBody::NodeChanged()
{
	if (node)
	{		
		rotation = node->GetWorldRotation();
		position = node->GetWorldPosition();
		node->GetComponents(shapes, true);
		for (auto &shape : shapes)
		{
			shape->SetOwnedByRigidBody(true);
		}
		UpdateMassProperties();
		worldBoundingBoxChanged = true;
	}
}

void RigidBody::UpdateCollisionShapes()
{
	shapes.clear();
	for (auto &shape : shapes)
	{
		shape->SetOwnedByRigidBody(false);
	}

	if (node)
	{
		node->GetComponents(shapes, true);		
		for (auto &shape : shapes)
		{
			shape->SetOwnedByRigidBody(true);
		}
	}

	UpdateMassProperties();
	worldBoundingBoxChanged = true;
}

void RigidBody::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		if (physicsWorld.Expired() == false)
		{			
			physicsWorld->RemoveRigidBody(this);
		}
	}

	if (scene)
	{
		physicsWorld = scene->GetRootNode()->GetComponent<PhysicsWorld>();
		physicsWorld->AddRigidBody(this);
	}
}

void RigidBody::SetVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

vec3 RigidBody::GetVelocity() const
{
	return velocity;
}

void RigidBody::RenderDebugGeometry(DebugRenderer *debugRenderer)
{

}

vec3 RigidBody::PointToLocal(vec3 p)
{
	p -= position;
	return rotation.Conjugate() * p;
}

vec3 RigidBody::VectorToLocal(vec3 v)
{
	return rotation.Conjugate() * v;
}

vec3 RigidBody::PointToWorld(vec3 p)
{
	return rotation * p + position;
}
vec3 RigidBody::VectorToWorld(vec3 v)
{
	return rotation * v;
}

Component *RigidBody::Clone() const
{
	RigidBody *clone = new RigidBody(*this);
	return clone;
}

void RigidBody::ApplyForce(vec3 force, vec3 relativePos)
{
	vec3 rotForce = Cross(relativePos, force);
	this->force += force;
	this->torque += rotForce;
}

void RigidBody::ApplyLocalForce(vec3 force, vec3 v)
{
	vec3 worldForce = VectorToWorld(force);
	vec3 relativePos = VectorToWorld(v);

	ApplyForce(worldForce, relativePos);
}

void RigidBody::ApplyImpulse(vec3 impulse, vec3 relativePos)
{
	this->velocity += impulse * invMass;
	vec3 rotVelocity = Cross(relativePos, impulse);
	this->angularVelocity += invInertiaWorld * rotVelocity;
}

void RigidBody::ApplyLocalImpulse(vec3 impulse, vec3 v)
{
	vec3 worldImpulse = VectorToWorld(impulse);
	vec3 relativePos = VectorToWorld(v);
	ApplyImpulse(worldImpulse, relativePos);
}

void RigidBody::Integrate(float t)
{
	if (bodyType != RigidBodyType::DYNAMIC) { // Only for dynamic
		return;
	}

	velocity += force * invMass * t;
	angularVelocity += invInertiaWorld * torque * t;
	position += velocity * t;

	float ax = angularVelocity.x,
		ay = angularVelocity.y,
		az = angularVelocity.z,
		bx = rotation.x,
		by = rotation.y,
		bz = rotation.z,
		bw = rotation.w;
	
	float halfT = t * 0.5f;
	rotation.x += halfT * (ax * bw + ay * bz - az * by);
	rotation.y += halfT * (ay * bw + az * bx - ax * bz);
	rotation.z += halfT * (az * bw + ax * by - ay * bx);
	rotation.w += halfT * (-ax * bx - ay * by - az * bz);

	rotation.Normalize();
	
	worldBoundingBoxChanged = true;

	UpdateInertiaWorld();
}

void RigidBody::UpdateInertiaWorld(bool force)
{
	vec3 I = invInertia;
	if (I.x == I.y && I.y == I.z && !force)
	{
		invInertiaWorld = mat3::IDENTITY;		
		invInertiaWorld.Scale(I.x, I.x, I.x);
	}
	else
	{
		mat3 m = rotation.GetMatrix();
		mat3 mt = m;
		mt.Transpose();
		m.Scale(I.x, I.y, I.z);
		invInertiaWorld = m * mt;
	}
}

void RigidBody::UpdateWorldBoundingBox()
{
	worldBoundingBoxChanged = false;

	worldBoundingBox = BoundingBox();
	for (auto shape : shapes)
	{
		auto shapeBBox = shape->GetWorldBoundingBox();
		worldBoundingBox.Expand(shapeBBox);
	}
}

void RigidBody::UpdateMassProperties()
{
	invMass = mass > 0.0f ? 1.0f / mass : 0.0f;	
	UpdateWorldBoundingBox();
	vec3 halfExtents = worldBoundingBox.GetSize() * 0.5f;
	
	inertia.x = 1.0f / 12.0f * mass * (2.0f * halfExtents.y * 2.0f * halfExtents.y + 2.0f * halfExtents.z * 2 * halfExtents.z);
	inertia.y = 1.0f / 12.0f * mass * (2.0f * halfExtents.x * 2.0f * halfExtents.x + 2.0f * halfExtents.z * 2 * halfExtents.z);
	inertia.z = 1.0f / 12.0f * mass * (2.0f * halfExtents.y * 2.0f * halfExtents.y + 2.0f * halfExtents.x * 2 * halfExtents.x);

	invInertia = vec3(inertia.x > 0 ? 1.0f / inertia.x : 0.0f, inertia.y > 0 ? 1.0f / inertia.y : 0.0f, inertia.z > 0 ? 1.0f / inertia.z : 0.0f);
	UpdateInertiaWorld(true);
}

void RigidBody::SetForce(vec3 force)
{
	this->force = force;
}

void RigidBody::SetTorque(vec3 torque)
{
	this->torque = torque;
}

void RigidBody::SetMass(float mass)
{
	this->mass = mass;
	if (mass == 0)
	{
		bodyType = RigidBodyType::STATIC;
	}
	else
	{
		bodyType = RigidBodyType::DYNAMIC;
	}
	UpdateMassProperties();
}

quat RigidBody::GetRotation() const
{
	return rotation;
}

vec3 RigidBody::GetPosition() const
{
	return position;
}

vec3 RigidBody::GetAngularVelocity() const
{
	return angularVelocity;
}

vec3 RigidBody::GetForce() const
{
	return force;
}

vec3 RigidBody::GetTorque() const
{
	return torque;
}

float RigidBody::GetInvMass() const
{
	return invMass;
}

mat3 RigidBody::GetInvInertiaWorld() const
{
	return invInertiaWorld;
}

vec3 RigidBody::GetVelocityAtWorldPoint(vec3 worldPoint) const
{
	vec3 r;
	r = worldPoint - position;
	
	vec3 result = Cross(angularVelocity, r) + velocity;	
	return result;
}

void RigidBody::SetAngularVelocity(vec3 angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

std::vector<CollisionShape *> & RigidBody::GetCollisionShapes()
{
	return shapes;
}

void RigidBody::SetBodyType(RigidBodyType bodyType)
{
	this->bodyType = bodyType;
}

RigidBodyType RigidBody::GetBodyType() const
{
	return bodyType;
}

void RigidBody::SetRotation(quat rotation)
{
	this->rotation = rotation;
}

void RigidBody::SetPosition(vec3 position)
{
	this->position = position;
}
