#include "Scene/SceneReader.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Geometry/Mesh.h"
#include "Geometry/GeometryGenerator.h"
#include "Scene/Scene.h"
#include "Scene/Node.h"
#include "Scene/StaticModel.h"
#include "Scene/Prefab.h"
#include "Scene/ParticleEmitter.h"
#include "Scene/Camera.h"
#include "Scene/Terrain.h"
#include "Utils/Image.h"
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
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(resolvedFileName))
		return false;	

	TiXmlElement *e = xmlDocument.FirstChildElement("scene");
	if (e == nullptr)
		return false;
	
	if (e->Attribute("fogColor"))
	{
		scene.SetFogColor(Variant::FromString(e->Attribute("fogColor")).GetVec3());
	}

	if (e->Attribute("ambientColor"))
	{
		scene.SetAmbientColor(Variant::FromString(e->Attribute("ambientColor")).GetVec3());
	}

	e = e->FirstChildElement("node");
	if (e == nullptr)
		return false;
	
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
		else if (element->ValueStr() == "prefab")
		{
			ReadPrefab(element, node);
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
			component->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>(e->Attribute("mesh")));
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

		TiXmlElement *materials = e->FirstChildElement("materials");
		if (materials)
		{
			for (TiXmlElement *material = materials->FirstChildElement("material"); material != nullptr; material = material->NextSiblingElement("material"))
			{
				unsigned int index = Variant::FromString(material->Attribute("index")).GetInt();
				std::string matName = material->Attribute("name");
				component->SetRenderablePartMaterial(index, engine->GetResourceCache()->GetResource<Material>(matName));
			}
		}
	}
	else if (type == "Box")
	{		
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		StaticModel *staticModel = nullptr;

		if (e->Attribute("size") && material)
		{
			staticModel = node->CreateComponent<StaticModel>();
			vec3 size = Variant::FromString(e->Attribute("size")).GetVec3();			
			Mesh mesh(engine);			
			mesh.AddGeometry(GeometryGenerator::GenerateBox(engine, size), BoundingBox(-size * 0.5f, size * 0.5f), material);
			staticModel->CreateFromMesh(&mesh);
		}

		if (staticModel)
		{			
			if (e->Attribute("cast_shadow"))
			{
				staticModel->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
			}

			if (e->Attribute("light_mask"))
			{
				staticModel->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
			}

			if (e->Attribute("collision_query_mask"))
			{
				staticModel->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
			}
		}
	}
	else if (type == "Plane")
	{
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		StaticModel *staticModel = nullptr;

		if (e->Attribute("size") && material)
		{
			staticModel = node->CreateComponent<StaticModel>();
			vec2 size = Variant::FromString(e->Attribute("size")).GetVec2();
			Mesh mesh(engine);
			mesh.AddGeometry(GeometryGenerator::GeneratePlane(engine, size), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), material);
			staticModel->CreateFromMesh(&mesh);
		}

		if (staticModel)
		{
			if (e->Attribute("cast_shadow"))
			{
				staticModel->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
			}

			if (e->Attribute("light_mask"))
			{
				staticModel->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
			}

			if (e->Attribute("collision_query_mask"))
			{
				staticModel->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
			}
		}
	}
	else if (type == "Sphere")
	{		
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		StaticModel *staticModel = nullptr;

		if (e->Attribute("radius") && material)
		{
			staticModel = node->CreateComponent<StaticModel>();
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
			staticModel->CreateFromMesh(&mesh);
		}

		if (staticModel)
		{
			if (e->Attribute("cast_shadow"))
			{
				staticModel->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
			}

			if (e->Attribute("light_mask"))
			{
				staticModel->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
			}

			if (e->Attribute("collision_query_mask"))
			{
				staticModel->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
			}
		}
	}
	else if (type == "LuaScript")
	{		
		if (e->Attribute("object"))
		{
			auto component = node->CreateComponent<LuaScript>();

			for (TiXmlElement *propertyElement = e->FirstChildElement("property"); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement("property"))
			{
				if (propertyElement->Attribute("name") && propertyElement->Attribute("value"))
				{
					component->SetInitialPropertyValue(propertyElement->Attribute("name"), propertyElement->Attribute("value"));
				}
			}

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
			light->SetColor(Variant::FromString(e->Attribute("color")).GetVec3());			
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
			else if (shapeTypeStr == "sphere")
			{
				float radius = Variant::FromString(e->Attribute("radius")).GetFloat();
				component->SetSphere(radius);
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

		if (e->Attribute("contact_offset"))
		{
			component->SetContactOffset(Variant::FromString(e->Attribute("contact_offset")).GetFloat());
		}

		if (e->Attribute("step_offset"))
		{
			component->SetStepOffset(Variant::FromString(e->Attribute("step_offset")).GetFloat());
		}
	}
	else if (type == "RigidBody")
	{	
		auto component = node->CreateComponent<RigidBody>();
		float mass = 0.0f;
		if (e->Attribute("mass"))
		{
			mass = Variant::FromString(e->Attribute("mass")).GetFloat();
		}

		component->SetMass(mass);					
	}
	else if (type == "ParticleEmitter")
	{		
		unsigned int numberOfParticles = (unsigned int) Variant::FromString(e->Attribute("number_of_particles")).GetFloat();
		Material *material = nullptr;
		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}
		auto component = node->CreateComponent<ParticleEmitter>(numberOfParticles, material);
		component->SetEmissionRate((unsigned int)Variant::FromString(e->Attribute("emission_rate")).GetFloat());
		vec2 lifeTime = Variant::FromString(e->Attribute("life_time")).GetVec2();
		component->SetLifeTime(lifeTime.x, lifeTime.y);
		vec2 speed = Variant::FromString(e->Attribute("speed")).GetVec2();
		component->SetSpeed(speed.x, speed.y);
		component->SetPrewarm(Variant::FromString(e->Attribute("prewarm")).GetBool());

		if (e->Attribute("simulation_space"))
		{
			std::string simulationSpace = e->Attribute("simulation_space");
			if (simulationSpace == "local")
			{
				component->SetSimulationSpace(ParticleEmitterSimulationSpace::LOCAL);
			}
			else if (simulationSpace == "world")
			{
				component->SetSimulationSpace(ParticleEmitterSimulationSpace::WORLD);
			}
		}

		std::string shape = e->Attribute("shape");
		if (shape == "box")
		{
			vec3 size = Variant::FromString(e->Attribute("box")).GetVec3();
			component->SetBoxEmitter(size);
		}
		else if (shape == "sphere")
		{
			float radius = Variant::FromString(e->Attribute("radius")).GetFloat();
			component->SetSphereEmitter(radius);
		}
	}
	else if (type == "Camera")
	{
		auto component = node->CreateComponent<Camera>();
		std::string projection = e->Attribute("projection");
		float nearPlane = Variant::FromString(e->Attribute("near")).GetFloat();
		float farPlane = Variant::FromString(e->Attribute("far")).GetFloat();
		if (projection == "orthographic")
		{
			float left = Variant::FromString(e->Attribute("left")).GetFloat();
			float right = Variant::FromString(e->Attribute("right")).GetFloat();
			float top = Variant::FromString(e->Attribute("top")).GetFloat();
			float bottom = Variant::FromString(e->Attribute("bottom")).GetFloat();

			component->SetOrthographicProjectionParameters(left, right, bottom, top, nearPlane, farPlane);
		}
		else if (projection == "perspective")
		{
			float fov = Variant::FromString(e->Attribute("fov")).GetFloat();
			component->SetPerspectiveProjectionParameters(fov, 1.0f, nearPlane, farPlane);
		}
	}
	else if (type == "Terrain")
	{
		auto component = node->CreateComponent<Terrain>();		

		if (e->Attribute("material"))
		{
			component->SetMaterial(engine->GetResourceCache()->GetResource<Material>(e->Attribute("material")));
		}

		if (e->Attribute("vertices_spacing"))
		{
			vec3 spacing = Variant::FromString(e->Attribute("vertices_spacing")).GetVec3();
			component->SetVerticesSpacing(spacing);
		}

		if (e->Attribute("generate_hard_normals"))
		{
			component->SetGenerateHardNormals(Variant::FromString(e->Attribute("generate_hard_normals")).GetBool());
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

		if (e->Attribute("heightmap"))
		{
			component->CreateFromHeightMap(engine->GetResourceCache()->GetResource<Image>(e->Attribute("heightmap")));
		}
	}
	else if (type == "Grid")
	{
		Material *material = nullptr;

		if (e->Attribute("material"))
		{
			material = engine->GetResourceCache()->GetResource<Material>(e->Attribute("material"));
		}

		StaticModel *staticModel = nullptr;

		if (e->Attribute("size") && e->Attribute("count_x") && e->Attribute("count_z") && material)
		{
			staticModel = node->CreateComponent<StaticModel>();
			vec2 size = Variant::FromString(e->Attribute("size")).GetVec2();
			unsigned int countX = std::stoul(e->Attribute("count_x"));
			unsigned int countZ = std::stoul(e->Attribute("count_z"));
			Mesh mesh(engine);
			mesh.AddGeometry(GeometryGenerator::GenerateGrid(engine, size, countX, countZ), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), material);
			staticModel->CreateFromMesh(&mesh);
		}

		if (staticModel)
		{
			if (e->Attribute("cast_shadow"))
			{
				staticModel->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
			}

			if (e->Attribute("light_mask"))
			{
				staticModel->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
			}

			if (e->Attribute("collision_query_mask"))
			{
				staticModel->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
			}
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
		node->SetRotation(quat(rotation.x, rotation.y, rotation.z));
	}
}

void SceneReader::ReadPrefab(TiXmlElement *e, Node *node)
{
	Prefab *prefab = nullptr;
	if (e->Attribute("name"))
	{
		prefab = engine->GetResourceCache()->GetResource<Prefab>(e->Attribute("name"));		
	}

	if (prefab)
	{
		auto prefabInstance = prefab->Instantiate();

		TiXmlElement *transformationElement = e->FirstChildElement("transformation");
		if (transformationElement)
		{
			ReadTransformation(transformationElement, prefabInstance);
		}
		prefabInstance->SetParent(node);
	}
}
