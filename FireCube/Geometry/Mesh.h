#pragma once
#include "Utils/utils.h"
#include "Core/Resource.h"
#include "Math/BoundingBox.h"
#include "Geometry/Material.h"

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace FireCube
{

class Engine;
class Geometry;

class FIRECUBE_API Mesh : public Resource
{
	OBJECT(Mesh)
public:
	Mesh(Engine *engine);
	
	virtual bool Load(const std::string &filename);
	bool LoadOld(const std::string &filename);
	const std::vector<SharedPtr<Geometry>> &GetGeometries() const;
	const std::vector<SharedPtr<Material>> &GetMaterials() const;
	void AddGeometry(Geometry *geometry, Material *material);
	const BoundingBox &GetBoundingBox() const;
	void SetBoundingBox(BoundingBox boundingBox);
private:
	void ProcessAssimpScene(const aiScene *aScene);
	SharedPtr<Material> ProcessAssimpMaterial(const aiMaterial *aMaterial);
	SharedPtr<Geometry> ProcessAssimpMesh(const aiMesh *aMesh, mat4 transformation);
	void ProcessAssimpNode(const aiScene *aScene, const aiNode *aNode, const std::vector<SharedPtr<Material>> &materialList, mat4 parentTransformation);
	BoundingBox boundingBox;
	std::vector<SharedPtr<Geometry>> geometries;
	std::vector<SharedPtr<Material>> materials;
};

}