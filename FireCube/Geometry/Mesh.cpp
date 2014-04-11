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

Mesh::~Mesh()
{
	for (auto i : geometries)
	{
		delete i->GetVertexBuffer();
		delete i->GetIndexBuffer();
		delete i;
	}
	
	std::vector<Material *> deletedMaterials;
	for (auto i : materials)
	{
		if (std::find(deletedMaterials.begin(), deletedMaterials.end(), i) == deletedMaterials.end())
		{
			deletedMaterials.push_back(i);
			delete i;
		}
	}
}

bool Mesh::Load(const std::string &filename)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return false;
	std::string::size_type d;
	d = filename.find_last_of(".");	
	std::ostringstream ss;
	ss << "Created model with name:" << filename;
	LOGINFO(ss.str());	
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
				geometries.push_back(modelLoader->GetGeneratedGeometries()[i]);				
				materials.push_back(modelLoader->GetGeneratedMaterials()[i]);				
			}
				
			boundingBox = modelLoader->GetBoundingBox();

			delete modelLoader;
			return true;
		}		
	}
	return false;
}

const std::vector<Geometry *> &Mesh::GetGeometries() const
{
	return geometries;
}

const std::vector<Material *> &Mesh::GetMaterials() const
{
	return materials;
}

const BoundingBox &Mesh::GetBoundingBox() const
{
	return boundingBox;
}