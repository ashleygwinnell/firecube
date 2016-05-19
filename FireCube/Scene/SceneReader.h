#pragma once

#include "Utils/utils.h"
#include "Core/Object.h"

class TiXmlElement;

namespace FireCube
{

class Scene;
class Node;

/**
* Reads a scene from an xml file
*/
class FIRECUBE_API SceneReader : Object
{
	FIRECUBE_OBJECT(SceneReader)
public:
	SceneReader(Engine *engine);
	
	/**
	* Reads a scene from an xml file
	* @param scene The scene to fill
	* @param filename The file name to load 
	*/
	bool Read(Scene &scene, const std::string &filename);
	
	/**
	* Reads a single node from an xml element
	* @param e The xml element
	* @param node The node to fill
	*/
	void ReadNode(TiXmlElement *e, Node *node);
private:
	
	void ReadComponent(TiXmlElement *e, Node *node);
	void ReadTransformation(TiXmlElement *e, Node *node);	
	void ReadPrefab(TiXmlElement *e, Node *node);
};

}