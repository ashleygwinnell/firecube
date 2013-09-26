#include <sstream>

#include "Rendering/StaticModel.h"
#include "Utils/Filesystem.h"
#include "Utils/Logger.h"
#include "Geometry/ColladaLoader.h"
#include "Geometry/m3dsLoader.h"
#include "Geometry/ObjLoader.h"
#include "Core/Engine.h"
using namespace FireCube;

StaticModel::StaticModel(Engine *engine) : Renderable(engine)
{
	
}

void StaticModel::LoadMesh(const std::string &filename, ModelLoadingOptions options)
{
	std::string file = Filesystem::SearchForFileName(filename);
	if (file.empty())
		return;
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

		if (modelLoader->Load(file, options))
		{
			modelLoader->GenerateGeometries(engine->GetRenderer());
			for (auto &geometry : modelLoader->GetGeneratedGeometries())
			{					
				RenderablePart part;
				part.geometry = geometry;
				renderableParts.push_back(part);
				geometries.push_back(GeometryPtr(geometry));
			}
			SetBoundingBox(modelLoader->GetBoundingBox());
		}		
		
		delete modelLoader;
	}	
}

void StaticModel::SetBoundingBox(BoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
	MarkedDirty();
}

void StaticModel::UpdateWorldBoundingBox()
{
	worldBoundingBox = boundingBox;
	worldBoundingBox.Transform(node->GetWorldTransformation());
}