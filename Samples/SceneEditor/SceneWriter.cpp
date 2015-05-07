#include "SceneWriter.h"
#include "tinyxml.h"
#include "SceneSettings.h"
#include "wx/filename.h"

using namespace FireCube;

void SceneWriter::Serialize(FireCube::Scene *scene, SceneSettings *sceneSettings, const std::string &filename)
{
	wxString path;
	wxFileName::SplitPath(filename, &path, nullptr, nullptr, wxPATH_NATIVE);

	basePath = path;
	TiXmlDocument doc;
	SerializeSettings(sceneSettings, &doc);
	TiXmlElement *element = new TiXmlElement("scene");	
	doc.LinkEndChild(element);
	Serialize(scene->GetRootNode(), element);
	doc.SaveFile(filename);
}

void SceneWriter::Serialize(FireCube::Node *node, TiXmlElement *parent)
{
	if (node->GetName().substr(0, 5) == "User_")
	{
		TiXmlElement *element = new TiXmlElement("node");
		parent->LinkEndChild(element);

		element->SetAttribute("name", node->GetName());
		SerializeNodeTransformation(node, element);

		for (auto component : node->GetComponents())
		{
			Serialize(component, element);
		}

		for (auto child : node->GetChildren())
		{
			Serialize(child, element);
		}
	}
	else
	{
		for (auto child : node->GetChildren())
		{
			Serialize(child, parent);
		}
	}
}

void SceneWriter::Serialize(FireCube::Component *component, TiXmlElement *parent)
{
	if (component->GetType() == StaticModel::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());

		auto staticModel = static_cast<StaticModel *>(component);

		wxFileName fileName(staticModel->GetMesh()->GetFileName());
		fileName.MakeRelativeTo(basePath);
		std::string file = fileName.GetFullPath();

		element->SetAttribute("mesh", file);
	}
}

void SceneWriter::SerializeNodeTransformation(FireCube::Node *node, TiXmlElement *parent)
{	
	TiXmlElement *transformation = new TiXmlElement("transformation");
	parent->LinkEndChild(transformation);	

	std::ostringstream translationAttribute, scaleAttribute, rotationAttribute;
	translationAttribute << node->GetTranslation().x << " " << node->GetTranslation().y << " " << node->GetTranslation().z;
	scaleAttribute << node->GetScale().x << " " << node->GetScale().y << " " << node->GetScale().z;
	vec3 eulerAngles = node->GetRotation().ExtractEulerAngles();
	rotationAttribute << eulerAngles.x << " " << eulerAngles.y << " " << eulerAngles.z;
	transformation->SetAttribute("translation", translationAttribute.str());
	transformation->SetAttribute("scale", scaleAttribute.str());
	transformation->SetAttribute("rotation", rotationAttribute.str());
}

void SceneWriter::SerializeSettings(SceneSettings *sceneSettings, TiXmlNode *parent)
{
	TiXmlElement *settings = new TiXmlElement("settings");
	parent->LinkEndChild(settings);
	
	for (const auto &resourcePath : sceneSettings->resourcePaths)
	{
		TiXmlElement *element = new TiXmlElement("resource_path");
		settings->LinkEndChild(element);
		TiXmlText *text = new TiXmlText(resourcePath.c_str());
		element->LinkEndChild(text);
	}	
}