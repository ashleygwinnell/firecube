#pragma once

#include "Renderable.h"

namespace FireCube
{

/**
* A renderable component which creates a box of unit size which is always located at the origin in world space
*/
class FIRECUBE_API Skybox : public Renderable
{
	FIRECUBE_OBJECT(Skybox)
public:
	Skybox(Engine *engine);
	Skybox(const Skybox &other);

	virtual void UpdateWorldBoundingBox();
	
	/**
	* Sets the material to use when rendering the box
	* @param material The material to use
	*/
	void SetMaterial(Material *material);
	
	/**
	* Clones this component
	*/
	Component *Clone() const;
private:
};

}