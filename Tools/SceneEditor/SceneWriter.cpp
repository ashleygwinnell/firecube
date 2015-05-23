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
		if (fileName.IsAbsolute())
		{
			fileName.MakeRelativeTo(basePath);
		}
		std::string file = fileName.GetFullPath();

		element->SetAttribute("mesh", file);
		element->SetAttribute("cast_shadow", staticModel->GetCastShadow() ? "true" : "false");

		std::stringstream stream;
		stream << std::hex << staticModel->GetLightMask();
		element->SetAttribute("light_mask", stream.str());
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
		
		std::stringstream stream;
		stream << std::hex << light->GetLightMask();
		element->SetAttribute("mask", stream.str());		
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
			if (fileName.IsAbsolute())
			{
				fileName.MakeRelativeTo(basePath);
			}
			std::string file = fileName.GetFullPath();

			element->SetAttribute("script", file);
		}

		element->SetAttribute("object", luaScript->GetObjectName());		
	}
	else if (component->GetType() == PhysicsWorld::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());
	}
	else if (component->GetType() == CollisionShape::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());

		auto collisionShape = static_cast<CollisionShape *>(component);

		element->SetAttribute("is_trigger", collisionShape->IsTrigger() ? "true" : "false");

		switch (collisionShape->GetShapeType())
		{
		case CollisionShapeType::BOX:
			element->SetAttribute("shape_type", "box");
			element->SetAttribute("bbox_min", ToString(collisionShape->GetBox().GetMin()));
			element->SetAttribute("bbox_max", ToString(collisionShape->GetBox().GetMax()));
			break;
		case CollisionShapeType::PLANE:
			element->SetAttribute("shape_type", "plane");
			element->SetAttribute("plane", ToString(vec4(collisionShape->GetPlane().GetNormal(), collisionShape->GetPlane().GetDistance())));
			break;
		case CollisionShapeType::TRIANGLE_MESH:
		{
			element->SetAttribute("shape_type", "triangle_mesh");
			wxFileName fileName(collisionShape->GetMesh()->GetFileName());
			if (fileName.IsAbsolute())
			{
				fileName.MakeRelativeTo(basePath);
			}
			std::string file = fileName.GetFullPath();

			element->SetAttribute("mesh", file);
			break;
		}
		default:
			break;
		}
	}
	else if (component->GetType() == CharacterController::GetTypeStatic())
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", component->GetTypeName());

		auto characterController = static_cast<CharacterController *>(component);
		element->SetAttribute("radius", ToString(characterController->GetRadius()));		
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
