#pragma once

#include "Core/Resource.h"

namespace FireCube
{

class Node;

/**
* A prefab node resource. The prefab is stored in an xml file
*/
class FIRECUBE_API Prefab : public Resource
{
	FIRECUBE_OBJECT(Prefab)
public:
	Prefab(Engine *engine);
	virtual ~Prefab();
	
	/**
	* Loads the prefab from an xml file
	* @param filename The file to load
	*/
	virtual bool Load(const std::string &filename);
	
	/**
	* Instantiates the prefab by returning a clone of the node loaded from xml
	* @returns The cloned node
	*/	
	Node *Instantiate() const;

	private:
		Node *node;
};

}