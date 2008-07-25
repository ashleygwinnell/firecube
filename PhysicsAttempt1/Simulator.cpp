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
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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
void Simulator::Render(Renderer &renderer)
{
	vector<RigidBody *>::iterator i=rigidBodies.begin();

	for (;i!=rigidBodies.end();i++)
	{
		(*i)->Render(renderer);
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
		b->force=b->mass*vec3(0,-0.2f,0);
	}
}
void Simulator::Update(float t)
{
	ResetForcesAndTorques();	
}