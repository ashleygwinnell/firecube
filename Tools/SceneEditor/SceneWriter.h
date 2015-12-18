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
private:
	std::string ToString(FireCube::vec2 v) const;
	std::string ToString(FireCube::vec3 v) const;
	std::string ToString(FireCube::vec4 v) const;	
	void Serialize(NodeDescriptor *nodeDesc, TiXmlElement *parent);
	void Serialize(ComponentDescriptor *componentDesc, TiXmlElement *parent);
	void SerializeNodeTransformation(NodeDescriptor *nodeDesc, TiXmlElement *parent);	
};