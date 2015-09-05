#pragma once

#include "FireCube.h"

class TiXmlElement;
class TiXmlNode;
class SceneSettings;

class SceneWriter
{
public:
	void Serialize(FireCube::Scene *scene, const std::string &filename);
private:
	std::string ToString(FireCube::vec3 v) const;
	std::string ToString(FireCube::vec4 v) const;	
	void Serialize(FireCube::Node *node, TiXmlElement *parent);
	void Serialize(FireCube::Component *component, TiXmlElement *parent);
	void SerializeNodeTransformation(FireCube::Node *node, TiXmlElement *parent);
	std::string basePath;
};