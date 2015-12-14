#pragma once

#include "Renderable.h"

namespace FireCube
{

class FIRECUBE_API Skybox : public Renderable
{
	FIRECUBE_OBJECT(Skybox)
public:
	Skybox(Engine *engine);
	Skybox(const Skybox &other);

	virtual void UpdateWorldBoundingBox();
	void SetMaterial(Material *material);
	Component *Clone() const;
private:
};

}