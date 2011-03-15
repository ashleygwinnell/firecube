#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "app.h"
#include "Grid3D.h"
#include "CollisionShape.h"
#include "utils.h"
#include "RigidBody.h"

void DetectBodyCollisions(RigidBody *body0, RigidBody *body1, vector<CollisionInfo> &collisions)
{
    int numPoints = body1->worldVertices.size();
    vec3 pos0;
    float radius0;
    body0->GetBoundingSphere(pos0, radius0);
    vec3 dir;
    float dist;

    for (int i = 0; i < numPoints; i++)
    {
        vec3 worldPos1 = body1->worldVertices[i];
        if ((worldPos1 - pos0).Length2() < radius0 * radius0)
        {
            vec3 pos1 = body0->inverseOrientation * (worldPos1 - body0->position);
            if (body0->collisionShape->GetPointInfo(pos1, dir, dist, false))
            {
                vec3 normal = dir * body0->orientaion;
                vec3 midPoint = worldPos1 + normal * dist * 0.5f;
                collisions.push_back(CollisionInfo(body0, body1, -normal, midPoint, dist));
                body0->collisions.push_back(collisions.size() - 1);
                body1->collisions.push_back(collisions.size() - 1);
            }
        }
    }
    int numEdges = body1->collisionShape->edge.size();
    for (int i = 0; i < numEdges; i++)
    {
        Edge &edge = body1->collisionShape->edge[i];
        const int numPoints = 6;
        vec3 pos1Start = body1->worldVertices[edge.v[0]];
        vec3 pos1End = body1->worldVertices[edge.v[1]];
        vec3 delta = pos1End - pos1Start;
        bool useOneCollisionPerEdge = true;
        if (useOneCollisionPerEdge)
        {
            vec3 bestNormal;
            vec3 bestPos;
            float bestDepth = -1000.0f;
            for (int j = 0; j < numPoints; j++)
            {
                vec3 worldPos1 = pos1Start + delta * (float)j / (float)numPoints;
                if ((worldPos1 - pos0).Length2() < radius0 * radius0)
                {
                    vec3 pos1 = body0->inverseOrientation * (worldPos1 - body0->position);
                    if (body0->collisionShape->GetPointInfo(pos1, dir, dist, false))
                    {
                        if (dist > bestDepth)
                        {
                            bestNormal = dir * body0->orientaion;
                            bestPos = worldPos1 + bestNormal * 0.5f * dist;
                            bestDepth = dist;
                        }
                    }
                    if (bestDepth >= 0.0f)
                    {
                        collisions.push_back(CollisionInfo(body0, body1, -bestNormal, bestPos, bestDepth));
                        body0->collisions.push_back(collisions.size() - 1);
                        body1->collisions.push_back(collisions.size() - 1);
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < numPoints; j++)
            {
                vec3 worldPos1 = pos1Start + delta * (float)j / (float)numPoints;
                if ((worldPos1 - pos0).Length2() < radius0 * radius0)
                {
                    vec3 pos1 = body0->inverseOrientation * (worldPos1 - body0->position);
                    if (body0->collisionShape->GetPointInfo(pos1, dir, dist, false))
                    {
                        vec3 normal = body0->orientaion * dir;
                        vec3 midPoint = worldPos1 + normal * dist * 0.5f;
                        collisions.push_back(CollisionInfo(body0, body1, -normal, midPoint, dist));
                        body0->collisions.push_back(collisions.size() - 1);
                        body1->collisions.push_back(collisions.size() - 1);
                    }
                }
            }
        }
    }
}
RigidBody::RigidBody()
{
    mass = 1.0f;
    invMass = 1.0f / mass;
}
void RigidBody::Init(Node m, CollisionShape *cs)
{
    model = m;
    collisionShape = cs;
    worldVertices.resize(cs->vertex.size());
}
void RigidBody::CalculateWorldProperties()
{
    inverseOrientation = orientaion;
    inverseOrientation.Inverse();
    int numPoints = worldVertices.size();
    for (int i = 0; i < numPoints; i++)
    {
        worldVertices[i] = collisionShape->vertex[i] * orientaion + position;
    }
}
bool RigidBody::GetPointInfo(vec3 pos, vec3 &dir, float &dist)
{
    vec3 bodyPos = (pos - position) * inverseOrientation;
    bool inside = collisionShape->GetPointInfo(bodyPos, dir, dist, false);
    dir = dir * orientaion;
    return inside;
}
void RigidBody::Render(mat4 transform)
{
    mat4 t;
    t.Translate(position);
    t *= orientaion.ToMat4();
    model->SetMatrixTransformation(transform * t);
    Renderer::Render(model);
}
void RigidBody::GetBoundingSphere(vec3 &pos, float &radius)
{
    pos = position;
    radius = collisionShape->radius;
}
