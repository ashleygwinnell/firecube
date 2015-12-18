#include "SceneWriter.h"
#include "tinyxml.h"
#include "wx/filename.h"
#include "wx/wx.h"
#include "app.h"
#include "Descriptors/NodeDescriptor.h"
#include "Descriptors/ComponentDescriptor.h"
#include "Descriptors/StaticModelDescriptor.h"
#include "Descriptors/LightDescriptor.h"
#include "Descriptors/PhysicsWorldDescriptor.h"
#include "Descriptors/CollisionShapeDescriptor.h"
#include "Descriptors/CharacterControllerDescriptor.h"
#include "Descriptors/LuaScriptDescriptor.h"
#include "Descriptors/BoxDescriptor.h"
#include "Descriptors/RigidBodyDescriptor.h"
#include "Descriptors/PlaneDescriptor.h"

using namespace FireCube;

void SceneWriter::Serialize(NodeDescriptor *root, const std::string &filename)
{	
	TiXmlDocument doc;	
	TiXmlElement *element = new TiXmlElement("scene");	
	doc.LinkEndChild(element);
	Serialize(root, element);
	doc.SaveFile(filename);
}

void SceneWriter::Serialize(NodeDescriptor *nodeDesc, TiXmlElement *parent)
{
	TiXmlElement *element = new TiXmlElement("node");
	parent->LinkEndChild(element);

	element->SetAttribute("name", nodeDesc->GetName());
	SerializeNodeTransformation(nodeDesc, element);

	for (auto component : nodeDesc->GetComponents())
	{
		Serialize(component, element);
	}

	for (auto child : nodeDesc->GetChildren())
	{
		Serialize(child, element);
	}
}

void SceneWriter::Serialize(ComponentDescriptor *componentDesc, TiXmlElement *parent)
{
	if (componentDesc->GetType() == ComponentType::STATIC_MODEL)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto staticModel = static_cast<StaticModelDescriptor *>(componentDesc);
		
		element->SetAttribute("mesh", staticModel->GetMeshFilename());
		element->SetAttribute("cast_shadow", staticModel->GetCastShadow() ? "true" : "false");

		std::stringstream ligtMaskStream;
		ligtMaskStream << std::hex << staticModel->GetLightMask();
		element->SetAttribute("light_mask", ligtMaskStream.str());

		std::stringstream collisionQueryMaskStream;
		collisionQueryMaskStream << std::hex << staticModel->GetCollisionQueryMask();
		element->SetAttribute("collision_query_mask", collisionQueryMaskStream.str());		
	}
	else if (componentDesc->GetType() == ComponentType::LIGHT)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto light = static_cast<LightDescriptor *>(componentDesc);		

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
	else if (componentDesc->GetType() == ComponentType::LUA_SCRIPT)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto luaScript = static_cast<LuaScriptDescriptor *>(componentDesc);

		if (luaScript->GetScriptFilename().empty() == false)
		{		
			element->SetAttribute("script", luaScript->GetScriptFilename());
		}

		element->SetAttribute("object", luaScript->GetObjectName());
		
		auto &properties = luaScript->GetProperties();
		for (auto &property : properties)
		{
			TiXmlElement *propertyElement = new TiXmlElement("property");
			element->LinkEndChild(propertyElement);

			propertyElement->SetAttribute("name", property.first);
			propertyElement->SetAttribute("value", property.second);
		}		
	}
	else if (componentDesc->GetType() == ComponentType::PHYSICS_WORLD)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetComponent()->GetTypeName());
	}
	else if (componentDesc->GetType() == ComponentType::COLLISION_SHAPE)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto collisionShape = static_cast<CollisionShapeDescriptor *>(componentDesc);

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
			element->SetAttribute("mesh", collisionShape->GetMeshFilename());
			break;
		}
		case CollisionShapeType::SPHERE:
			element->SetAttribute("shape_type", "sphere");
			element->SetDoubleAttribute("radius", collisionShape->GetRadius());
			break;
		default:
			break;
		}
	}
	else if (componentDesc->GetType() == ComponentType::CHARACTER_CONTROLLER)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto characterController = static_cast<CharacterControllerDescriptor *>(componentDesc);
		element->SetDoubleAttribute("radius", characterController->GetRadius());
		element->SetDoubleAttribute("height", characterController->GetHeight());
		element->SetDoubleAttribute("contact_offset", characterController->GetContactOffset());
		element->SetDoubleAttribute("step_offset", characterController->GetStepOffset());
	}
	else if (componentDesc->GetType() == ComponentType::BOX)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto box = static_cast<BoxDescriptor *>(componentDesc);

		element->SetAttribute("size", ToString(box->GetSize()));
		element->SetAttribute("material", box->GetMaterialFileName());
		element->SetAttribute("cast_shadow", box->GetCastShadow() ? "true" : "false");

		std::stringstream ligtMaskStream;
		ligtMaskStream << std::hex << box->GetLightMask();
		element->SetAttribute("light_mask", ligtMaskStream.str());

		std::stringstream collisionQueryMaskStream;
		collisionQueryMaskStream << std::hex << box->GetCollisionQueryMask();
		element->SetAttribute("collision_query_mask", collisionQueryMaskStream.str());
	}
	else if (componentDesc->GetType() == ComponentType::PLANE)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto plane = static_cast<PlaneDescriptor *>(componentDesc);

		element->SetAttribute("size", ToString(plane->GetSize()));
		element->SetAttribute("material", plane->GetMaterialFileName());
		element->SetAttribute("cast_shadow", plane->GetCastShadow() ? "true" : "false");

		std::stringstream ligtMaskStream;
		ligtMaskStream << std::hex << plane->GetLightMask();
		element->SetAttribute("light_mask", ligtMaskStream.str());

		std::stringstream collisionQueryMaskStream;
		collisionQueryMaskStream << std::hex << plane->GetCollisionQueryMask();
		element->SetAttribute("collision_query_mask", collisionQueryMaskStream.str());
	}
	else if (componentDesc->GetType() == ComponentType::RIGID_BODY)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto rigidBody = static_cast<RigidBodyDescriptor *>(componentDesc);

		element->SetDoubleAttribute("mass", rigidBody->GetMass());		
	}

}

void SceneWriter::SerializeNodeTransformation(NodeDescriptor *nodeDesc, TiXmlElement *parent)
{	
	TiXmlElement *transformation = new TiXmlElement("transformation");
	parent->LinkEndChild(transformation);	
		
	transformation->SetAttribute("translation", ToString(nodeDesc->GetTranslation()));
	transformation->SetAttribute("scale", ToString(nodeDesc->GetScale()));
	transformation->SetAttribute("rotation", ToString(nodeDesc->GetRotation()));
}

std::string SceneWriter::ToString(vec2 v) const
{
	std::ostringstream str;
	str << v.x << " " << v.y;
	return str.str();
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
