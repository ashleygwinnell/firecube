#pragma once

#include "FireCube.h"

class TiXmlElement;
class TiXmlNode;
class SceneSettings;
class NodeDescriptor;
class ComponentDescriptor;

class SceneWriter
{
public:
	void Serialize(NodeDescriptor *root, const std::string &filename);
	void SerializePrefab(NodeDescriptor *root, const std::string &filename);
	void SerializePrefab(NodeDescriptor *root, const std::string &filename, FireCube::vec3 rootRotation, FireCube::vec3 rootScale);
private:
	std::string ToString(FireCube::vec2 v) const;
	std::string ToString(FireCube::vec3 v) const;
	std::string ToString(FireCube::vec4 v) const;	
	void Serialize(NodeDescriptor *nodeDesc, bool prefabAsNode, TiXmlNode *parent);
	void Serialize(ComponentDescriptor *componentDesc, TiXmlNode *parent);
	void SerializeNodeTransformation(NodeDescriptor *nodeDesc, TiXmlNode *parent, bool zeroTranslation = false);
};