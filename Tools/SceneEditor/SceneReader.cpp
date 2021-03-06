#include "SceneReader.h"
#include "tinyxml.h"
#include <iostream>
#include "Descriptors/NodeDescriptor.h"
#include "EditorState.h"
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
#include "Descriptors/GridDescriptor.h"

using namespace FireCube;

::SceneReader::SceneReader(Engine *engine) : Object(engine)
{

}

bool ::SceneReader::Read(NodeDescriptor *root, const std::string &filename)
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

	Scene *scene = root->GetNode()->GetScene();
	if (e->Attribute("fogColor"))
	{
		scene->SetFogColor(Variant::FromString(e->Attribute("fogColor")).GetVec3());
	}

	if (e->Attribute("ambientColor"))
	{
		scene->SetAmbientColor(Variant::FromString(e->Attribute("ambientColor")).GetVec3());
	}

	e = e->FirstChildElement("node");
	if (e == nullptr)
		return false;
	
	ReadNode(e, root);	

	if (e->NextSiblingElement())
	{
		LOGWARNING("Ignoring sibling nodes of root node when reading scene file: ", filename);
	}

	return true;
}

void ::SceneReader::ReadNode(TiXmlElement *e, NodeDescriptor *node)
{
	if (e->Attribute("name"))
	{
		node->SetName(e->Attribute("name"));
	}

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "node")
		{
			NodeDescriptor *child = new NodeDescriptor();			
			child->SetParent(node);
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

void ::SceneReader::ReadComponent(TiXmlElement *e, NodeDescriptor *node)
{
	if (e->Attribute("type") == nullptr)
	{
		LOGERROR("No component type specified");
		return;
	}

	std::string  type = e->Attribute("type");
	ComponentDescriptor *addedComponent = nullptr;
	if (type == "StaticModel")
	{
		StaticModelDescriptor *staticModelDescriptor = new StaticModelDescriptor();
		addedComponent = staticModelDescriptor;
		if (e->Attribute("mesh"))
		{
			staticModelDescriptor->SetMeshFilename(e->Attribute("mesh"), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			staticModelDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());			
		}

		if (e->Attribute("light_mask"))
		{
			staticModelDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));			
		}

		if (e->Attribute("collision_query_mask"))
		{
			staticModelDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));			
		}

		TiXmlElement *materials = e->FirstChildElement("materials");
		if (materials)
		{
			for (TiXmlElement *material = materials->FirstChildElement("material"); material != nullptr; material = material->NextSiblingElement("material"))
			{
				unsigned int index = std::stoi(material->Attribute("index"));
				std::string matName = material->Attribute("name");
				staticModelDescriptor->SetRenderablePartMaterial(index, matName);
			}
		}
	}
	else if (type == "Box")
	{
		BoxDescriptor *boxDescriptor = new BoxDescriptor();
		addedComponent = boxDescriptor;		

		if (e->Attribute("size"))
		{			
			vec3 size = Variant::FromString(e->Attribute("size")).GetVec3();
			boxDescriptor->SetSize(size, engine);
		}

		if (e->Attribute("material"))
		{			
			boxDescriptor->SetMaterialFileName(e->Attribute("material"), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			boxDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			boxDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			boxDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}
	}
	else if (type == "Plane")
	{
		PlaneDescriptor *planeDescriptor = new PlaneDescriptor();
		addedComponent = planeDescriptor;

		if (e->Attribute("size"))
		{
			vec2 size = Variant::FromString(e->Attribute("size")).GetVec2();
			planeDescriptor->SetSize(size, engine);
		}

		if (e->Attribute("material"))
		{
			planeDescriptor->SetMaterialFileName(e->Attribute("material"), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			planeDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			planeDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			planeDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}
	}
	else if (type == "Sphere")
	{
		SphereDescriptor *sphereDescriptor = new SphereDescriptor();
		addedComponent = sphereDescriptor;

		if (e->Attribute("material"))
		{
			sphereDescriptor->SetMaterialFileName(e->Attribute("material"), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			sphereDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			sphereDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			sphereDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}

		if (e->Attribute("radius"))
		{
			sphereDescriptor->SetRadius(Variant::FromString(e->Attribute("radius")).GetFloat(), engine);
		}

		unsigned int rings = 16;
		unsigned int columns = 16;
		if (e->Attribute("tessellation"))
		{
			vec2 t = Variant::FromString(e->Attribute("tessellation")).GetVec2();
			rings = (unsigned int)t.x;
			columns = (unsigned int)t.y;
		}
		
		sphereDescriptor->SetRings(rings, engine);
		sphereDescriptor->SetColumns(columns, engine);
	}
	else if (type == "LuaScript")
	{
		if (e->Attribute("object"))
		{
			auto luaScriptDescriptor = new LuaScriptDescriptor();
			addedComponent = luaScriptDescriptor;

			luaScriptDescriptor->SetObjectName(e->Attribute("object"));
			if (e->Attribute("script"))
			{
				luaScriptDescriptor->SetScriptFilename(e->Attribute("script"));				
			}			

			for (TiXmlElement *propertyElement = e->FirstChildElement("property"); propertyElement != nullptr; propertyElement = propertyElement->NextSiblingElement("property"))
			{
				if (propertyElement->Attribute("name") && propertyElement->Attribute("value"))
				{
					luaScriptDescriptor->SetProperty(propertyElement->Attribute("name"), propertyElement->Attribute("value"));					
				}
			}
		}
		else
		{
			LOGERROR("No script object name specified");
		}
	}
	else if (type == "Light")
	{
		auto lightDescriptor = new LightDescriptor();
		addedComponent = lightDescriptor;
		if (e->Attribute("light_type"))
		{
			std::string lightTypeStr = e->Attribute("light_type");
			if (lightTypeStr == "directional")
			{
				lightDescriptor->SetLightType(LightType::DIRECTIONAL);
			}
			else if (lightTypeStr == "point")
			{
				lightDescriptor->SetLightType(LightType::POINT);
			}
			else if (lightTypeStr == "spot")
			{
				lightDescriptor->SetLightType(LightType::SPOT);
			}
		}

		if (e->Attribute("color"))
		{
			lightDescriptor->SetColor(Variant::FromString(e->Attribute("color")).GetVec3());
		}

		if (e->Attribute("range"))
		{
			lightDescriptor->SetRange(Variant::FromString(e->Attribute("range")).GetFloat());
		}

		if (e->Attribute("spot_cutoff"))
		{
			lightDescriptor->SetSpotCutOff(Variant::FromString(e->Attribute("spot_cutoff")).GetFloat());
		}

		if (e->Attribute("shadow_intensity"))
		{
			lightDescriptor->SetShadowIntensity(Variant::FromString(e->Attribute("shadow_intensity")).GetFloat());
		}

		if (e->Attribute("cast_shadow"))
		{
			lightDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("mask"))
		{
			lightDescriptor->SetLightMask(std::stoul(e->Attribute("mask"), 0, 16));
		}		
	}
	else if (type == "PhysicsWorld")
	{
		auto physicsWorldDescriptor = new PhysicsWorldDescriptor();
		addedComponent = physicsWorldDescriptor;
	}
	else if (type == "CollisionShape")
	{
		auto collisionShapeDescriptor = new CollisionShapeDescriptor();
		addedComponent = collisionShapeDescriptor;

		if (e->Attribute("shape_type"))
		{
			std::string shapeTypeStr = e->Attribute("shape_type");
			if (shapeTypeStr == "triangle_mesh")
			{
				collisionShapeDescriptor->SetMesh(e->Attribute("mesh"), engine);
			}
			else if (shapeTypeStr == "box")
			{
				vec3 bboxMin = Variant::FromString(e->Attribute("bbox_min")).GetVec3();
				vec3 bboxMax = Variant::FromString(e->Attribute("bbox_max")).GetVec3();
				collisionShapeDescriptor->SetBox(BoundingBox(bboxMin, bboxMax));
			}
			else if (shapeTypeStr == "plane")
			{
				vec4 planeParams = Variant::FromString(e->Attribute("plane")).GetVec4();
				Plane plane(planeParams.ToVec3(), planeParams.w);
				collisionShapeDescriptor->SetPlane(plane);
			}
			else if (shapeTypeStr == "sphere")
			{
				float radius = Variant::FromString(e->Attribute("radius")).GetFloat();
				collisionShapeDescriptor->SetSphere(radius);
			}
		}

		if (e->Attribute("is_trigger"))
		{
			collisionShapeDescriptor->SetIsTrigger(Variant::FromString(e->Attribute("is_trigger")).GetBool());
		}
	}
	else if (type == "CharacterController")
	{
		auto characterControllerDescriptor = new CharacterControllerDescriptor();
		addedComponent = characterControllerDescriptor;
		if (e->Attribute("radius"))
		{
			characterControllerDescriptor->SetRadius(Variant::FromString(e->Attribute("radius")).GetFloat());
		}

		if (e->Attribute("height"))
		{
			characterControllerDescriptor->SetHeight(Variant::FromString(e->Attribute("height")).GetFloat());
		}

		if (e->Attribute("contact_offset"))
		{
			characterControllerDescriptor->SetContactOffset(Variant::FromString(e->Attribute("contact_offset")).GetFloat());
		}

		if (e->Attribute("step_offset"))
		{
			characterControllerDescriptor->SetStepOffset(Variant::FromString(e->Attribute("step_offset")).GetFloat());
		}
	}
	else if (type == "RigidBody")
	{
		auto rigidBodyDescriptor = new RigidBodyDescriptor();
		addedComponent = rigidBodyDescriptor;
		float mass = 0.0f;
		if (e->Attribute("mass"))
		{
			mass = Variant::FromString(e->Attribute("mass")).GetFloat();
		}

		rigidBodyDescriptor->SetMass(mass);
	}
	else if (type == "ParticleEmitter")
	{
		auto particleEmitterDescriptor = new ParticleEmitterDescriptor();
		addedComponent = particleEmitterDescriptor;		

		particleEmitterDescriptor->SetNumberOfParticles((unsigned int) Variant::FromString(e->Attribute("number_of_particles")).GetFloat());		
		if (e->Attribute("material"))
		{
			particleEmitterDescriptor->SetMaterial(e->Attribute("material"), engine);
		}
		
		particleEmitterDescriptor->SetEmissionRate((unsigned int) Variant::FromString(e->Attribute("emission_rate")).GetFloat());
		vec2 lifeTime = Variant::FromString(e->Attribute("life_time")).GetVec2();
		particleEmitterDescriptor->SetLifeTime(lifeTime.x, lifeTime.y);
		vec2 speed = Variant::FromString(e->Attribute("speed")).GetVec2();
		particleEmitterDescriptor->SetSpeed(speed.x, speed.y);		
		particleEmitterDescriptor->SetPrewarm(Variant::FromString(e->Attribute("prewarm")).GetBool());

		if (e->Attribute("simulation_space"))
		{
			std::string simulationSpace = e->Attribute("simulation_space");
			if (simulationSpace == "local")
			{
				particleEmitterDescriptor->SetSimulationSpace(ParticleEmitterSimulationSpace::LOCAL);
			}
			else if (simulationSpace == "world")
			{
				particleEmitterDescriptor->SetSimulationSpace(ParticleEmitterSimulationSpace::WORLD);
			}
		}

		std::string shape = e->Attribute("shape");
		if (shape == "box")
		{
			vec3 size = Variant::FromString(e->Attribute("box")).GetVec3();
			particleEmitterDescriptor->SetBoxEmitter(size);
		}
		else if (shape == "sphere")
		{
			float radius = Variant::FromString(e->Attribute("radius")).GetFloat();
			particleEmitterDescriptor->SetSphereEmitter(radius);
		}		
	}
	else if (type == "Camera")
	{
		auto cameraDescriptor = new CameraDescriptor();
		addedComponent = cameraDescriptor;

		std::string projection = e->Attribute("projection");
		cameraDescriptor->SetNearPlane(Variant::FromString(e->Attribute("near")).GetFloat());		
		cameraDescriptor->SetFarPlane(Variant::FromString(e->Attribute("far")).GetFloat());
		if (projection == "orthographic")
		{
			cameraDescriptor->SetOrthographic(true);
			cameraDescriptor->SetLeftPlane(Variant::FromString(e->Attribute("left")).GetFloat());
			cameraDescriptor->SetRightPlane(Variant::FromString(e->Attribute("right")).GetFloat());
			cameraDescriptor->SetTopPlane(Variant::FromString(e->Attribute("top")).GetFloat());
			cameraDescriptor->SetBottomPlane(Variant::FromString(e->Attribute("bottom")).GetFloat());
		}
		else if (projection == "perspective")
		{
			cameraDescriptor->SetOrthographic(false);
			cameraDescriptor->SetFOV(Variant::FromString(e->Attribute("fov")).GetFloat());			
		}
	}
	else if (type == "Terrain")
	{
		TerrainDescriptor *terrainDescriptor = new TerrainDescriptor();
		addedComponent = terrainDescriptor;
		if (e->Attribute("heightmap"))
		{
			terrainDescriptor->SetHeightmapFilename(e->Attribute("heightmap"), engine);
		}

		if (e->Attribute("material"))
		{
			terrainDescriptor->SetMaterialFileName(e->Attribute("material"), engine);
		}

		if (e->Attribute("vertices_spacing"))
		{
			terrainDescriptor->SetVerticesSpacing(Variant::FromString(e->Attribute("vertices_spacing")).GetVec3(), engine);
		}

		if (e->Attribute("generate_hard_normals"))
		{
			terrainDescriptor->SetGenerateHardNormals(Variant::FromString(e->Attribute("generate_hard_normals")).GetBool(), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			terrainDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			terrainDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			terrainDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}
	}
	else if (type == "Grid")
	{
		GridDescriptor *gridDescriptor = new GridDescriptor();
		addedComponent = gridDescriptor;

		if (e->Attribute("size"))
		{
			vec2 size = Variant::FromString(e->Attribute("size")).GetVec2();
			gridDescriptor->SetSize(size, engine);
		}

		if (e->Attribute("count_x"))
		{
			gridDescriptor->SetCountX(std::stoul(e->Attribute("count_x")), engine);
		}

		if (e->Attribute("count_z"))
		{
			gridDescriptor->SetCountZ(std::stoul(e->Attribute("count_z")), engine);
		}

		if (e->Attribute("material"))
		{
			gridDescriptor->SetMaterialFileName(e->Attribute("material"), engine);
		}

		if (e->Attribute("cast_shadow"))
		{
			gridDescriptor->SetCastShadow(Variant::FromString(e->Attribute("cast_shadow")).GetBool());
		}

		if (e->Attribute("light_mask"))
		{
			gridDescriptor->SetLightMask(std::stoul(e->Attribute("light_mask"), 0, 16));
		}

		if (e->Attribute("collision_query_mask"))
		{
			gridDescriptor->SetCollisionQueryMask(std::stoul(e->Attribute("collision_query_mask"), 0, 16));
		}
	}
	else
	{
		LOGERROR("Unknown component type: ", type);
	}

	if (addedComponent)
	{
		node->AddComponent(addedComponent);
		addedComponent->SetParent(node);
		if (node->GetNode())
		{
			addedComponent->CreateComponent(node->GetNode(), engine);
		}
	}
}

void ::SceneReader::ReadTransformation(TiXmlElement *e, NodeDescriptor *node)
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
		node->SetRotation(rotation);
	}
}

void ::SceneReader::ReadPrefab(TiXmlElement * element, NodeDescriptor * node)
{
	NodeDescriptor *prefab = nullptr;
	if (element->Attribute("name"))
	{
		std::string resolvedFileName = Filesystem::FindResourceByName(element->Attribute("name"));
		if (resolvedFileName.empty())
			return;

		TiXmlDocument xmlDocument;
		if (!xmlDocument.LoadFile(resolvedFileName))
			return;

		TiXmlElement *e = xmlDocument.FirstChildElement("node");
		if (e == nullptr)
			return;

		prefab = new NodeDescriptor();

		::SceneReader reader(engine);
		reader.ReadNode(e, prefab);	
		prefab->SetIsPrefab(true);
		prefab->SetPrefabPath(element->Attribute("name"));
	}

	if (prefab)
	{
		auto prefabInstance = prefab->Clone();

		TiXmlElement *transformationElement = element->FirstChildElement("transformation");
		if (transformationElement)
		{
			ReadTransformation(transformationElement, prefabInstance);
		}
		
		prefabInstance->SetParent(node);
	}
}

NodeDescriptor *::SceneReader::ReadPrefab(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return nullptr;

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(resolvedFileName))
		return nullptr;

	TiXmlElement *e = xmlDocument.FirstChildElement("node");
	if (e == nullptr)
		return nullptr;

	NodeDescriptor *prefab = new NodeDescriptor();

	::SceneReader reader(engine);
	reader.ReadNode(e, prefab);
	prefab->SetIsPrefab(true);
	prefab->SetPrefabPath(filename);
	return prefab;
}
