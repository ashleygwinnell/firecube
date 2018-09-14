#include "SceneWriter.h"
#include "tinyxml.h"
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
#include "Descriptors/SphereDescriptor.h"
#include "Descriptors/ParticleEmitterDescriptor.h"
#include "Descriptors/CameraDescriptor.h"
#include "Descriptors/TerrainDescriptor.h"

using namespace FireCube;

void SceneWriter::Serialize(NodeDescriptor *root, const std::string &filename)
{	
	TiXmlDocument doc;	
	TiXmlElement *element = new TiXmlElement("scene");	
	doc.LinkEndChild(element);
	Serialize(root, false, element);
	doc.SaveFile(filename);
}

void SceneWriter::SerializePrefab(NodeDescriptor *root, const std::string &filename)
{
	TiXmlDocument doc;
	Serialize(root, true, &doc);
	doc.SaveFile(filename);
}

void SceneWriter::Serialize(NodeDescriptor *nodeDesc, bool prefabAsNode, TiXmlNode *parent)
{
	if (nodeDesc->IsPrefab() == false || prefabAsNode)
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
			Serialize(child, prefabAsNode, element);
		}
	}
	else
	{
		TiXmlElement *element = new TiXmlElement("prefab");
		parent->LinkEndChild(element);
		element->SetAttribute("name", nodeDesc->GetPrefabPath());
		SerializeNodeTransformation(nodeDesc, element);
	}
}

void SceneWriter::Serialize(ComponentDescriptor *componentDesc, TiXmlNode *parent)
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
	else if (componentDesc->GetType() == ComponentType::SPHERE)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto sphere = static_cast<SphereDescriptor *>(componentDesc);

		element->SetDoubleAttribute("radius", sphere->GetRadius());
		element->SetAttribute("tessellation", ToString(vec2((float) sphere->GetRings(), (float) sphere->GetColumns())));
		element->SetAttribute("material", sphere->GetMaterialFileName());
		element->SetAttribute("cast_shadow", sphere->GetCastShadow() ? "true" : "false");

		std::stringstream ligtMaskStream;
		ligtMaskStream << std::hex << sphere->GetLightMask();
		element->SetAttribute("light_mask", ligtMaskStream.str());

		std::stringstream collisionQueryMaskStream;
		collisionQueryMaskStream << std::hex << sphere->GetCollisionQueryMask();
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
	else if (componentDesc->GetType() == ComponentType::PARTICLE_EMITTER)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto particleEmitter = static_cast<ParticleEmitterDescriptor *>(componentDesc);	

		element->SetAttribute("number_of_particles", particleEmitter->GetNumberOfParticles());
		element->SetAttribute("emission_rate", particleEmitter->GetEmissionRate());
		element->SetAttribute("life_time", ToString(vec2(particleEmitter->GetMinLifeTime(), particleEmitter->GetMaxLifeTime())));
		element->SetAttribute("speed", ToString(vec2(particleEmitter->GetMinSpeed(), particleEmitter->GetMaxSpeed())));
		element->SetAttribute("material", particleEmitter->GetMaterial());
		element->SetAttribute("prewarm", particleEmitter->GetPrewarm() ? "true" : "false");

		switch (particleEmitter->GetEmitterShape())
		{
		case ParticleEmitterShape::BOX:
			element->SetAttribute("shape", "box");
			element->SetAttribute("box", ToString(particleEmitter->GetBox()));
			break;
		case ParticleEmitterShape::SPHERE:
			element->SetAttribute("shape", "sphere");
			element->SetDoubleAttribute("radius", particleEmitter->GetRadius());
			break;
		default:
			break;
		}

		switch (particleEmitter->GetSimulationSpace())
		{
		case ParticleEmitterSimulationSpace::LOCAL:
			element->SetAttribute("simulation_space", "local");
			break;
		case ParticleEmitterSimulationSpace::WORLD:
			element->SetAttribute("simulation_space", "world");
			break;
		default:
			break;
		}
	}
	else if (componentDesc->GetType() == ComponentType::CAMERA)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto camera = static_cast<CameraDescriptor *>(componentDesc);

		element->SetAttribute("projection", camera->GetOrthographic() ? "orthographic" : "perspective");
		if (camera->GetOrthographic())
		{
			element->SetDoubleAttribute("left", camera->GetLeftPlane());
			element->SetDoubleAttribute("right", camera->GetRightPlane());
			element->SetDoubleAttribute("top", camera->GetTopPlane());
			element->SetDoubleAttribute("bottom", camera->GetBottomPlane());
		}
		else
		{
			element->SetDoubleAttribute("fov", camera->GetFOV());			
		}

		element->SetDoubleAttribute("near", camera->GetNearPlane());
		element->SetDoubleAttribute("far", camera->GetFarPlane());		
	}
	else if (componentDesc->GetType() == ComponentType::TERRAIN)
	{
		TiXmlElement *element = new TiXmlElement("component");
		parent->LinkEndChild(element);

		element->SetAttribute("type", componentDesc->GetTypeName());

		auto terrain = static_cast<TerrainDescriptor *>(componentDesc);

		element->SetAttribute("heightmap", terrain->GetHeightmapFilename());
		element->SetAttribute("material", terrain->GetMaterialFileName());
		element->SetAttribute("vertices_spacing", ToString(terrain->GetVerticesSpacing()));
		element->SetAttribute("generate_hard_normals", terrain->GetGenerateHardNormals() ? "true" : "false");
		element->SetAttribute("cast_shadow", terrain->GetCastShadow() ? "true" : "false");

		std::stringstream ligtMaskStream;
		ligtMaskStream << std::hex << terrain->GetLightMask();
		element->SetAttribute("light_mask", ligtMaskStream.str());

		std::stringstream collisionQueryMaskStream;
		collisionQueryMaskStream << std::hex << terrain->GetCollisionQueryMask();
		element->SetAttribute("collision_query_mask", collisionQueryMaskStream.str());
	}
}

void SceneWriter::SerializeNodeTransformation(NodeDescriptor *nodeDesc, TiXmlNode *parent)
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
