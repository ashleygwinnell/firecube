#ifndef RIGID_BODY_H
#define RIGID_BODY_H
class RigidBody
{
public:
    RigidBody();
    void CalculateWorldProperties();
    void Init(NodePtr m, CollisionShape *cs);
    bool GetPointInfo(vec3 pos, vec3 &dir, float &dist);
    void GetBoundingSphere(vec3 &pos, float &radius);
    void Render(mat4 transform);

    mat3 orientaion;
    mat3 inverseOrientation;
    vec3 position;
    vec3 velocity;
    vec3 rotation;
    float mass;
    float invMass;
    mat3 bodyInertia;
    mat3 worldInertia;
    mat3 bodyInvInertia;
    mat3 worldInvInertia;

    float elasticity;
    float staticFriction;
    float dynamicFriction;

    vec3 force;
    vec3 torque;

    CollisionShape *collisionShape;
    NodePtr model;
    std::vector<vec3> worldVertices;
    std::vector<int> collisions;
};
class CollisionInfo
{
public:
    CollisionInfo() {}
    CollisionInfo(RigidBody *body0, RigidBody *body1, vec3 dir_to_0, vec3 position, float penetration_depth) :
        body0(body0), body1(body1), dirToBody0(dir_to_0), position(position), penetrationDepth(penetration_depth)
    {}
    RigidBody *body0;
    RigidBody *body1;
    vec3 dirToBody0;
    vec3 position;
    float penetrationDepth;
};
void DetectBodyCollisions(RigidBody *body0, RigidBody *body1, std::vector<CollisionInfo> &collisions);
#endif