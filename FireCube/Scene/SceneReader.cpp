#include "Scene/SceneReader.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Geometry/Mesh.h"
#include "Geometry/GeometryGenerator.h"
#include "Scene/Scene.h"
#include "Scene/Node.h"
#include "Scene/StaticModel.h"
#include "Utils/Filesystem.h"
#include "Scripting/LuaScript.h"
#include "Scripting/LuaFile.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/CollisionShape.h"
#include "Physics/CharacterController.h"
#include "tinyxml.h"
#include <iostream>

using namespace FireCube;

SceneReader::SceneReader(Engine *engine) : Object(engine)
{

}

bool SceneReader::Read(Scene &scene, const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(Filesystem::SearchForFileName(filename)))
		return false;	

	TiXmlElement *e = xmlDocument.FirstChildElement("scene");
	if (e == nullptr)
		return false;

	e = e->FirstChildElement("node");
	if (e == nullptr)
		return false;

	auto i0 = filename.find_last_of('\\');
	auto i1 = filename.find_last_of('/');
	auto i = i0;
	if (i0 == std::string::npos)
	{
		i = i1;
	}
	else if (i1 != std::string::npos)
	{
		i = std::max(i0, i1);
	}

	if (i != std::string::npos)
	{
		basePath = filename.substr(0, i + 1);
	}
	
	Node *node = scene.GetRootNode();
	ReadNode(e, node);
	
	if (e->NextSiblingElement())
	{
		LOGWARNING("Ignoring sibling nodes of root node when reading scene file: ", filename);
	}

	return true;
}

void SceneReader::ReadNode(TiXmlElement *e, Node *node)
{
	if (e->Attribute("name"))
	{
		node->SetName(e->Attribute("name"));
	}

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{	
		if (element->ValueStr() == "node")
		{
			Node *child = node->CreateChild();
			ReadNode(element, child);
		}
		else if (element->ValueStr() == "component")
		{
			ReadComponent(element, node);
		}
		else if (element->ValueStr() == "transformation")
		{
			ReadTransformation(element, node);
		}
	}
}

void SceneReader::ReadComponent(TiXmlElement *e, Node *node)
{
	if (e->Attribute("type") == nullptr)
	{
		LOGERROR("No component type specified");
		return;
	}

	std::string  type = e->Attribute("type");
	
	if (type == "StaticModel")
	{
		auto component = node->CreateComponent<StaticModel>();
		
		if (e->Attribute("mesh"))
		{
			component->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>(basePath + e->Attribute("mesh")));
		}		

		if (e->Attribute("cast_shadow"))
		{
			component->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			component->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			component->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}
	}
	else if (type == "box")
	{		
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		if (e->Attribute("size") && material)
		{
			auto component = node->CreateComponent<StaticModel>();
			vec3 size = Variant::FromString(e->Attribute("size")).GetVec3();			
			Mesh mesh(engine);			
			mesh.AddGeometry(GeometryGenerator::GenerateBox(engine, size), BoundingBox(-size, size), material);
			component->CreateFromMesh(&mesh);
		}
	}
	else if (type == "sphere")
	{		
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		if (e->Attribute("radius") && material)
		{
			auto component = node->CreateComponent<StaticModel>();
			float radius = Variant::FromString(e->Attribute("radius")).GetFloat();
			
			unsigned int rings = 16;
			unsigned int columns = 16;
			if (e->Attribute("tessellation"))
			{
				vec2 t = Variant::FromString(e->Attribute("tessellation")).GetVec2();
				rings = (unsigned int) t.x;
				columns = (unsigned int) t.y;
			}

			Mesh mesh(engine);
			mesh.AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, rings, columns), BoundingBox(-vec3(radius), vec3(radius)), material);			
			component->CreateFromMesh(&mesh);
		}
	}
	else if (type == "LuaScript")
	{		
		if (e->Attribute("object"))
		{
			auto component = node->CreateComponent<LuaScript>();
			if (e->Attribute("script"))
			{
				component->CreateObject(engine->GetResourceCache()->GetResource<LuaFile>(e->Attribute("script")), e->Attribute("object"));
			}
			else
			{
				component->CreateObject(e->Attribute("object"));
			}			
		}
		else
		{
			LOGERROR("No script object name specified");
		}
	}
	else if (type == "Light")
	{
		auto light = node->CreateComponent<Light>();
		if (e->Attribute("light_type"))
		{
			std::string lightTypeStr = e->Attribute("light_type");
			if (lightTypeStr == "directional")
			{
				light->SetLightType(LightType::DIRECTIONAL);
			}
			else if (lightTypeStr == "point")
			{
				light->SetLightType(LightType::POINT);
			}
			else if (lightTypeStr == "spot")
			{
				light->SetLightType(LightType::SPOT);
			}
		}		

		if (e->Attribute("color"))
		{
			light->SetColor(Variant::FromString(e->Attribute("color")).GetVec4());			
		}

		if (e->Attribute("range"))
		{
			light->SetRange(Variant::FromString(e->Attribute("range")).GetFloat());
		}

		if (e->Attribute("spot_cutoff"))
		{
			light->SetSpotCutOff(Variant::FromString(e->Attribute("spot_cutoff")).GetFloat());
		}

		if (e->Attribute("shadow_intensity"))
		{
			light->SetShadowIntensity(Variant::FromString(e->Attribute("shadow_intensity")).GetFloat());
		}

		if (e->Attribute("cast_shadow"))
		{
			light->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("mask"))
		{
			light->SetLightMask(std::stoul(e->Attribute("mask"), 0, 16));
		}
	}
	else if (type == "PhysicsWorld")
	{		
		node->CreateComponent<PhysicsWorld>();		
	}
	else if (type == "CollisionShape")
	{
		auto component = node->CreateComponent<CollisionShape>();
		if (e->Attribute("shape_type"))
		{
			std::string shapeTypeStr = e->Attribute("shape_type");
			if (shapeTypeStr == "triangle_mesh")
			{
				component->SetMesh(engine->GetResourceCache()->GetResource<Mesh>(e->Attribute("mesh")));
			}
			else if (shapeTypeStr == "box")
			{
				vec3 bboxMin = Variant::FromString(e->Attribute("bbox_min")).GetVec3();
				vec3 bboxMax = Variant::FromString(e->Attribute("bbox_max")).GetVec3();
				component->SetBox(BoundingBox(bboxMin, bboxMax));
			}
			else if (shapeTypeStr == "plane")
			{
				vec4 planeParams = Variant::FromString(e->Attribute("plane")).GetVec4();
				Plane plane(planeParams.ToVec3(), planeParams.w);
				component->SetPlane(plane);
			}
		}	

		if (e->Attribute("is_trigger"))
		{
			component->SetIsTrigger(Variant::FromString(e->Attribute("is_trigger")).GetBool());
		}
	}
	else if (type == "CharacterController")
	{
		auto component = node->CreateComponent<CharacterController>();
		if (e->Attribute("radius"))
		{
			component->SetRadius(Variant::FromString(e->Attribute("radius")).GetFloat());
		}

		if (e->Attribute("height"))
		{
			component->SetHeight(Variant::FromString(e->Attribute("height")).GetFloat());
		}
	}
	else
	{
		LOGERROR("Unknow component type: ", type);
	}
}

void SceneReader::ReadTransformation(TiXmlElement *e, Node *node)
{
	if (e->Attribute("translation"))
	{
		vec3 translation = Variant::FromString(e->Attribute("translation")).GetVec3();
		node->SetTranslation(translation);
	}
	
	if (e->Attribute("scale"))
	{
		vec3 scale = Variant::FromString(e->Attribute("scale")).GetVec3();
		node->SetScale(scale);
	}

	if (e->Attribute("rotation"))
	{
		vec3 rotation = Variant::FromString(e->Attribute("rotation")).GetVec3();		
		mat4 rotationMat = mat4::IDENTITY;
		rotationMat.RotateX(rotation.x);
		rotationMat.RotateY(rotation.y);
		rotationMat.RotateZ(rotation.z);
		node->SetRotation(rotationMat);
	}
}

void SceneReader::ReadSettings(TiXmlElement *e)
{
	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "resource_path")
		{
			resourcePaths.push_back(element->GetText());
		}
	}
}

void SceneReader::ReadSettings(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(Filesystem::SearchForFileName(filename)))
		return;

	TiXmlElement *settingsElement = xmlDocument.FirstChildElement("settings");
	if (settingsElement)
	{
		ReadSettings(settingsElement);
	}
}

const std::vector<std::string> &SceneReader::GetResroucePaths() const
{
	return resourcePaths;
}