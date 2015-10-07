#pragma once

#include "FireCube.h"

class TiXmlElement;

class AuxDataMap;

class SceneReader : FireCube::Object
{
	OBJECT(SceneReader)
public:
	SceneReader(FireCube::Engine *engine, AuxDataMap *auxDataMap);
	bool Read(FireCube::Scene &scene, const std::string &filename);
	
private:
	void ReadNode(TiXmlElement *e, FireCube::Node *node);
	void ReadComponent(TiXmlElement *e, FireCube::Node *node);
	void ReadTransformation(TiXmlElement *e, FireCube::Node *node);

	AuxDataMap *auxDataMap;
};