#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "app.h"
#include "Grid3D.h"
#include "CollisionShape.h"
#include "utils.h"
#include "RigidBody.h"
#include "Simulator.h"

Simulator::Simulator()
{

}
void Simulator::Render(mat4 transform)
{
    vector<RigidBody *>::iterator i = rigidBodies.begin();

    for (; i != rigidBodies.end(); i++)
    {
        (*i)->Render(transform);
    }
}
void Simulator::Add(RigidBody *body)
{
    rigidBodies.push_back(body);
}
void Simulator::ResetForcesAndTorques()
{
    vector<RigidBody *>::iterator i = rigidBodies.begin();

    for (; i != rigidBodies.end(); i++)
    {
        RigidBody *b = *i;
        b->torque = vec3(0, 0, 0);
        b->force = vec3(0, 0, 0);
    }
}
void Simulator::Update(float t)
{
    ResetForcesAndTorques();
}