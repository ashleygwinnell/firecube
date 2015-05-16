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
	if (node->GetName().substr(0, 7) != "Editor_")
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
	else if (component->GetType() == Light::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());

		auto light = static_cast<Light *>(component);		

		std::string lightTypeStr;

		switch (light->GetLightType())
		{
		case LightType::DIRECTIONAL:
			lightTypeStr = "directional";
			break;
		case LightType::POINT:
			lightTypeStr = "point";
			break;
		case LightType::SPOT:
			lightTypeStr = "spot";
			break;
		default:
			break;
		}

		element->SetAttribute("light_type", lightTypeStr);		
		element->SetAttribute("color", ToString(light->GetColor()));
		element->SetDoubleAttribute("range", light->GetRange());
		element->SetDoubleAttribute("spot_cutoff", light->GetSpotCutOff());
		element->SetDoubleAttribute("shadow_intensity", light->GetShadowIntensity());
		element->SetAttribute("cast_shadow", light->GetCastShadow() ? "true" : "false");
	}
	else if (component->GetType() == LuaScript::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());

		auto luaScript = static_cast<LuaScript *>(component);

		if (luaScript->GetLuaFile())
		{
			wxFileName fileName(luaScript->GetLuaFile()->GetFileName());
			fileName.MakeRelativeTo(basePath);
			std::string file = fileName.GetFullPath();

			element->SetAttribute("script", file);
		}

		element->SetAttribute("object", luaScript->GetObjectName());		
	}

}

void SceneWriter::SerializeNodeTransformation(FireCube::Node *node, TiXmlElement *parent)
{	
	TiXmlElement *transformation = new TiXmlElement("transformation");
	parent->LinkEndChild(transformation);	

	vec3 eulerAngles = node->GetRotation().ExtractEulerAngles();
	transformation->SetAttribute("translation", ToString(node->GetTranslation()));
	transformation->SetAttribute("scale", ToString(node->GetScale()));
	transformation->SetAttribute("rotation", ToString(eulerAngles));
}

void SceneWriter::SerializeSettings(SceneSettings *sceneSettings, TiXmlNode *parent)
{
	TiXmlElement *settings = new TiXmlElement("settings");
	parent->LinkEndChild(settings);
	
	for (const auto &resourcePath : sceneSettings->resourcePaths)
	{
		TiXmlElement *element = new TiXmlElement("resource_path");
		settings->LinkEndChild(element);

		wxFileName resourcePathRelative(basePath + "\\" + resourcePath, "");
		resourcePathRelative.MakeRelativeTo(basePath);		

		TiXmlText *text = new TiXmlText(resourcePathRelative.GetFullPath().ToStdString());
		element->LinkEndChild(text);
	}	
}

std::string SceneWriter::ToString(vec3 v) const
{
	std::ostringstream str;
	str << v.x << " " << v.y << " " << v.z;
	return str.str();
}

std::string SceneWriter::ToString(vec4 v) const
{
	std::ostringstream str;
	str << v.x << " " << v.y << " " << v.z << " " << v.w;
	return str.str();
}
