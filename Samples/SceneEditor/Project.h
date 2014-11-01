#pragma once

#include "FireCube.h"

class Project
{
public:
	Project();
	~Project();
	void Init(FireCube::Engine *engine);
	FireCube::Scene *GetScene();

	FireCube::Scene *scene;
};