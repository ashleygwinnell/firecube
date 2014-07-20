#include <sstream>

#include "Mesh.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Geometry/ModelLoader.h"
#include "ModelLoaders/ColladaLoader.h"
#include "ModelLoaders/m3dsLoader.h"
#include "ModelLoaders/ObjLoader.h"
#include "Core/Engine.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
using namespace FireCube;

Mesh::Mesh(Engine *engine) : Resource(engine)
{

}

bool Mesh::Load(const std::string &filename)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return false;
	std::string::size_type d;
	d = filename.find_last_of(".");		
	LOGINFO("Created model with name:", filename);
	if (d != std::string::npos)
	{
		std::string ext = ToLower(filename.substr(d + 1));
		ModelLoader *modelLoader = nullptr;
		if (ext == "3ds")
			modelLoader = new M3dsLoader(engine);		
		else if (ext == "dae")		
			modelLoader = new ColladaLoader(engine);
		else if (ext == "obj")
			modelLoader = new ObjLoader(engine);

		if (modelLoader && modelLoader->Load(file))
		{
			modelLoader->GenerateGeometries(engine->GetRenderer());						
			for (unsigned int i = 0; i < modelLoader->GetGeneratedGeometries().size(); ++i)
			{
				geometries.push_back(SharedPtr<Geometry>(modelLoader->GetGeneratedGeometries()[i]));
				materials.push_back(SharedPtr<Material>(modelLoader->GetGeneratedMaterials()[i]));
			}
				
			boundingBox = modelLoader->GetBoundingBox();

			delete modelLoader;
			return true;
		}		
	}
	return false;
}

const std::vector<SharedPtr<Geometry>> &Mesh::GetGeometries() const
{
	return geometries;
}

const std::vector<SharedPtr<Material>> &Mesh::GetMaterials() const
{
	return materials;
}

void Mesh::AddGeometry(Geometry *geometry, Material *material)
{
	geometries.push_back(geometry);
	materials.push_back(material);
}

const BoundingBox &Mesh::GetBoundingBox() const
{
	return boundingBox;
}

void Mesh::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
}