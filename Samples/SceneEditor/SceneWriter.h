#pragma once

#include "FireCube.h"

class TiXmlElement;
class TiXmlNode;
class SceneSettings;

class SceneWriter
{
public:
	void Serialize(FireCube::Scene *scene, SceneSettings *sceneSettings, const std::string &filename);
private:
	void SerializeSettings(SceneSettings *sceneSettings, TiXmlNode *parent);
	void Serialize(FireCube::Node *node, TiXmlElement *parent);
	void Serialize(FireCube::Component *component, TiXmlElement *parent);
	void SerializeNodeTransformation(FireCube::Node *node, TiXmlElement *parent);
	std::string basePath;
};