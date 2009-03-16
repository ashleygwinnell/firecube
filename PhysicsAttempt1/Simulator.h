#ifndef SIMULATOR_H
#define SIMULATOR_H
class Simulator
{
public:
	Simulator();
	
	void Add(RigidBody *body);
	void Render();
	void Update(float t);
	void ResetForcesAndTorques();

	vector<RigidBody *> rigidBodies;
};
#endif