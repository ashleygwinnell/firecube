#pragma once

#include "FireCube.h"

class TiXmlElement;

class NodeDescriptor;
class EditorState;

class SceneReader : FireCube::Object
{
	OBJECT(SceneReader)
public:
	SceneReader(FireCube::Engine *engine, EditorState *editorState);
	bool Read(NodeDescriptor *root, const std::string &filename);

private:
	void ReadNode(TiXmlElement *e, NodeDescriptor *node);
	void ReadComponent(TiXmlElement *e, NodeDescriptor *node);
	void ReadTransformation(TiXmlElement *e, NodeDescriptor *node);
	
	EditorState *editorState;
};