#pragma once

#include "FireCube.h"

class TiXmlElement;

class NodeDescriptor;
class EditorState;

class SceneReader : FireCube::Object
{
	FIRECUBE_OBJECT(SceneReader)
public:
	SceneReader(FireCube::Engine *engine, EditorState *editorState);
	bool Read(NodeDescriptor *root, const std::string &filename);
	void ReadNode(TiXmlElement *e, NodeDescriptor *node);
	NodeDescriptor *ReadPrefab(const std::string &filename);
private:

	void ReadComponent(TiXmlElement *e, NodeDescriptor *node);
	void ReadTransformation(TiXmlElement *e, NodeDescriptor *node);
	void ReadPrefab(TiXmlElement * element, NodeDescriptor * node);

	EditorState *editorState;
};