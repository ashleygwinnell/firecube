#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <cmath>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <windows.h>
using namespace std;
#include <gl/gl.h>
#include <FireCube.h>
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
void Simulator::Render()
{
	vector<RigidBody *>::iterator i=rigidBodies.begin();

	for (;i!=rigidBodies.end();i++)
	{
		(*i)->Render();
	}
}
void Simulator::Add(RigidBody *body)
{
	rigidBodies.push_back(body);
}
void Simulator::ResetForcesAndTorques()
{
	vector<RigidBody *>::iterator i=rigidBodies.begin();

	for (;i!=rigidBodies.end();i++)
	{
		RigidBody *b=*i;
		b->torque=vec3(0,0,0);
		b->force=vec3(0,0,0);
	}
}
void Simulator::Update(float t)
{
	ResetForcesAndTorques();	
}