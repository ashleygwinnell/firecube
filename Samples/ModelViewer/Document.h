#ifndef DOCUMENT_H
#define DOCUMENT_H

class Document
{
public:
	
	Document();
	~Document();
	bool Load(const std::string &filename, FireCube::Engine *engine);
	FireCube::Node *GetRoot();
	FireCube::Scene *GetScene();
	std::vector<FireCube::SharedPtr<FireCube::Material>> GetAllMaterials();
	void GenerateNormals(float l);
	void GenerateTangents(float l);

	bool HasTangents() const;
	unsigned int GetVertexCount() const;
	unsigned int GetFaceCount() const;
	void CreateRootNode(FireCube::Engine *engine);
	void CreateGrid(float size, DWORD numberOfCells);
	FireCube::CustomGeometry *GetNormalsGeometry();
	FireCube::CustomGeometry *GetTangentsGeometry();	
private:
	void GenerateNormals(FireCube::Node *node, float l, std::vector<FireCube::vec3> &normals);
	void GenerateTangents(FireCube::Node *node, float l, std::vector<FireCube::vec3> &tangents);
	
	void CountElements(unsigned int &verticesCount, unsigned int &facesCount);

	FireCube::Scene *scene;
	FireCube::Node *root;
	FireCube::Node *modelNode;
	FireCube::StaticModel *model;
	FireCube::Node *gridNode;
	FireCube::CustomGeometry *gridGeometry, *normalsGeometry, *tangentsGeometry;
	FireCube::SharedPtr<FireCube::Material> gridMaterial, tangentsMaterial;
	
	bool hasTangents;
	unsigned int verticesCount;
	unsigned int facesCount;
};

#endif