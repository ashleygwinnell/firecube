#include "Project.h"
#include "tinyxml.h"
#include "EditorState.h"
#include "FireCube.h"

using namespace FireCube;

void CProject::Load(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(filename))
		return;

	TiXmlElement *settingsElement = xmlDocument.FirstChildElement("settings");
	if (settingsElement == nullptr)
		return;

	TiXmlElement *e = settingsElement->FirstChildElement("last_scene_file");
	if (e)
	{
		lastSceneFile = e->Attribute("path");
	}

	e = settingsElement->FirstChildElement("game_executable_path");
	if (e)
	{
		gameExecutablePath = e->Attribute("path");
	}	
}

void CProject::Save(EditorState *editorState, const std::string &filename)
{
	TiXmlDocument doc;
	TiXmlElement *settingsElement = new TiXmlElement("settings");
	doc.LinkEndChild(settingsElement);

	lastSceneFile = editorState->GetCurrentSceneFile();
	TiXmlElement *element = new TiXmlElement("last_scene_file");
	settingsElement->LinkEndChild(element);
	element->SetAttribute("path", lastSceneFile);

	element = new TiXmlElement("game_executable_path");
	settingsElement->LinkEndChild(element);
	element->SetAttribute("path", gameExecutablePath);	

	doc.SaveFile(filename);
}

