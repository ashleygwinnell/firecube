#pragma once
#include <memory>
#include "Core/Object.h"

namespace FireCube
{

class Engine;

class Resource : public Object
{
	OBJECT(Resource)
public:
	Resource(Engine *engine);
	virtual bool Load(const std::string &filename) = 0;
	std::string GetFileName() const;
protected:
	std::string filename;
};

}