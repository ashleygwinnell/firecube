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
	void GenerateBitangents(float l);
	//FireCube::BufferPtr GetNormalRenderingBuffer();
	//FireCube::BufferPtr GetTangentRenderingBuffer();
	//FireCube::BufferPtr GetBitangentRenderingBuffer();
	int GetNormalRenderingBufferSize();	
	bool HasTangents() const;
	unsigned int GetVertexCount() const;
	unsigned int GetFaceCount() const;
	void CreateRootNode(FireCube::Engine *engine);
	void CreateGrid(float size, DWORD numberOfCells);
private:
	void GenerateNormals(FireCube::Node *node, float l, std::vector<FireCube::vec3> &normals);
	void GenerateTangents(FireCube::Node *node, float l, std::vector<FireCube::vec3> &tangents);
	void GenerateBitangents(FireCube::Node *node, float l, std::vector<FireCube::vec3> &bitangents);
	void CountElements(unsigned int &verticesCount, unsigned int &facesCount);

	FireCube::Scene *scene;
	FireCube::Node *root;
	FireCube::Node *modelNode;
	FireCube::StaticModel *model;
	FireCube::Node *gridNode;
	FireCube::CustomGeometry *gridGeometry;
	FireCube::Material *gridMaterial;
	//FireCube::BufferPtr normalRenderingBuffer;
	//FireCube::BufferPtr tangentRenderingBuffer;
	//FireCube::BufferPtr bitangentRenderingBuffer;
	int normalRenderingBufferSize;
	bool hasTangents;
	unsigned int verticesCount;
	unsigned int facesCount;
};

#endif