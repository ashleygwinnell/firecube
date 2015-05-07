#pragma once

#include "FireCube.h"

class TiXmlElement;

class SceneWriter
{
public:
	void Serialize(FireCube::Scene *scene, const std::string &filename);
private:
	void Serialize(FireCube::Node *node, TiXmlElement *parent);
	void Serialize(FireCube::Component *component, TiXmlElement *parent);
	void SerializeNodeTransformation(FireCube::Node *node, TiXmlElement *parent);
	std::string basePath;
};