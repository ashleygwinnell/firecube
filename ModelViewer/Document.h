#ifndef DOCUMENT_H
#define DOCUMENT_H

class Document
{
public:
	bool Load(const string &filename);
	FireCube::NodePtr GetRoot();
	void GenerateNormals(float l);
	void GenerateTangents(float l);
	void GenerateBitangents(float l);
	FireCube::BufferPtr GetNormalRenderingBuffer();
	FireCube::BufferPtr GetTangentRenderingBuffer();
	FireCube::BufferPtr GetBitangentRenderingBuffer();
	int GetNormalRenderingBufferSize();	
	bool HasTangents() const;
	unsigned int GetVertexCount() const;
	unsigned int GetFaceCount() const;
private:
	void GenerateNormals(FireCube::NodePtr node, float l, vector<FireCube::vec3> &normals);
	void GenerateTangents(FireCube::NodePtr node, float l, vector<FireCube::vec3> &tangents);
	void GenerateBitangents(FireCube::NodePtr node, float l, vector<FireCube::vec3> &bitangents);
	void CountElements(FireCube::NodePtr node, unsigned int &verticesCount, unsigned int &facesCount);

	FireCube::NodePtr root;
	FireCube::BufferPtr normalRenderingBuffer;
	FireCube::BufferPtr tangentRenderingBuffer;
	FireCube::BufferPtr bitangentRenderingBuffer;
	int normalRenderingBufferSize;
	bool hasTangents;
	unsigned int verticesCount;
	unsigned int facesCount;
};

#endif