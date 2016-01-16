#pragma once

#include "Utils/utils.h"
#include "Core/Object.h"

class TiXmlElement;

namespace FireCube
{

class Scene;
class Node;

class FIRECUBE_API SceneReader : Object
{
	FIRECUBE_OBJECT(SceneReader)
public:
	SceneReader(Engine *engine);
	bool Read(Scene &scene, const std::string &filename);
	void ReadNode(TiXmlElement *e, Node *node);
private:
	
	void ReadComponent(TiXmlElement *e, Node *node);
	void ReadTransformation(TiXmlElement *e, Node *node);	
	void ReadPrefab(TiXmlElement *e, Node *node);
};

}