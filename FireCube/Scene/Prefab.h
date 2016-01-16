#pragma once

#include "Core/Resource.h"

namespace FireCube
{

class Node;

class FIRECUBE_API Prefab : public Resource
{
	FIRECUBE_OBJECT(Prefab)
public:
	Prefab(Engine *engine);
	virtual ~Prefab();
	virtual bool Load(const std::string &filename);
	const Node *GetNode() const;

	private:
		Node *node;
};

}