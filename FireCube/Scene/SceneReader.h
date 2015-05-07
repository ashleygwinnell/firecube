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
	OBJECT(SceneReader)
public:
	SceneReader(Engine *engine);
	bool Read(Scene &scene, const std::string &filename);
	const std::vector<std::string> &GetResroucePaths() const;
	void ReadSettings(const std::string &filename);
private:
	void ReadNode(TiXmlElement *e, Node *node);
	void ReadComponent(TiXmlElement *e, Node *node);
	void ReadTransformation(TiXmlElement *e, Node *node);
	void ReadSettings(TiXmlElement *e);
	std::vector<std::string> resourcePaths;
};

}