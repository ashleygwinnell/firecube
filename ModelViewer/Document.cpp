#include <FireCube.h>
using namespace std;
#include "Document.h"

bool Document::Load(const string &filename)
{
	root = FireCube::LoadMesh(filename);
	FireCube::LightNodePtr l(new FireCube::LightNode("LightNode"));
	root->AddChild(l);	
	l->GetLight().SetAmbientColor(FireCube::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	l->GetLight().SetDiffuseColor(FireCube::vec4(0.7f, 0.7f, 0.7f, 1.0f));
	l->GetLight().SetSpecularColor(FireCube::vec4(0.3f, 0.3f, 0.3f, 1.0f));
	l->GetLight().SetType(FireCube::DIRECTIONAL);	
	l->Rotate(FireCube::vec3((float)PI / 4.0f, (float)PI / 4.0f, 0));

	verticesCount = 0;
	facesCount = 0;
	CountElements(root, verticesCount, facesCount);
	normalRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	tangentRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	bitangentRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	normalRenderingBuffer->Create();
	tangentRenderingBuffer->Create();
	bitangentRenderingBuffer->Create();	
	
	return true;
}
FireCube::NodePtr Document::GetRoot()
{
	return root;
}
void Document::CountElements(FireCube::NodePtr node, unsigned int &verticesCount, unsigned int &facesCount)
{
	if (node->GetGeometry())
	{
		verticesCount += node->GetGeometry()->GetVertices().size();
		facesCount += node->GetGeometry()->GetFaces().size();
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		CountElements(*i, verticesCount, facesCount);
}
void Document::GenerateNormals(float l)
{
	vector<FireCube::vec3> normals;
	GenerateNormals(root, l, normals);
	normalRenderingBuffer->LoadData(&normals[0], normals.size()*sizeof(FireCube::vec3), FireCube::STATIC);
	normalRenderingBufferSize = normals.size();
}
void Document::GenerateNormals(FireCube::NodePtr node, float l, vector<FireCube::vec3> &normals)
{
	if (node->GetGeometry())
	{
		for (unsigned int i = 0; i < node->GetGeometry()->GetVertices().size(); i++)
		{
			normals.push_back(node->GetGeometry()->GetVertices()[i]);
			normals.push_back(node->GetGeometry()->GetVertices()[i] + node->GetGeometry()->GetNormals()[i]*l);
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateNormals(*i, l, normals);
}
void Document::GenerateTangents(float l)
{
	vector<FireCube::vec3> tangents;
	GenerateTangents(root, l, tangents);
	if (!tangents.empty())
	{
		tangentRenderingBuffer->LoadData(&tangents[0], tangents.size()*sizeof(FireCube::vec3), FireCube::STATIC);
		hasTangents = true;
	}
	else
		hasTangents = false;
}
void Document::GenerateTangents(FireCube::NodePtr node, float l, vector<FireCube::vec3> &tangents)
{
	if (node->GetGeometry() && !node->GetGeometry()->GetTangents().empty())
	{
		for (unsigned int i = 0; i < node->GetGeometry()->GetVertices().size(); i++)
		{
			tangents.push_back(node->GetGeometry()->GetVertices()[i]);
			tangents.push_back(node->GetGeometry()->GetVertices()[i] + node->GetGeometry()->GetTangents()[i]*l);
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateTangents(*i, l, tangents);
}
void Document::GenerateBitangents(float l)
{
	vector<FireCube::vec3> bitangents;
	GenerateBitangents(root, l, bitangents);
	if (!bitangents.empty())
		bitangentRenderingBuffer->LoadData(&bitangents[0], bitangents.size()*sizeof(FireCube::vec3), FireCube::STATIC);
}
void Document::GenerateBitangents(FireCube::NodePtr node, float l, vector<FireCube::vec3> &bitangents)
{
	if (node->GetGeometry() && !node->GetGeometry()->GetBitangents().empty())
	{
		for (unsigned int i = 0; i < node->GetGeometry()->GetVertices().size(); i++)
		{
			bitangents.push_back(node->GetGeometry()->GetVertices()[i]);
			bitangents.push_back(node->GetGeometry()->GetVertices()[i] + node->GetGeometry()->GetBitangents()[i]*l);
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateBitangents(*i, l, bitangents);
}
bool Document::HasTangents() const
{
	return hasTangents;
}
unsigned int Document::GetVertexCount() const
{
	return verticesCount;
}
unsigned int Document::GetFaceCount() const
{
	return facesCount;
}
FireCube::BufferPtr Document::GetNormalRenderingBuffer()
{
	return normalRenderingBuffer;
}
FireCube::BufferPtr Document::GetTangentRenderingBuffer()
{
	return tangentRenderingBuffer;
}
FireCube::BufferPtr Document::GetBitangentRenderingBuffer()
{
	return bitangentRenderingBuffer;
}

int Document::GetNormalRenderingBufferSize()
{
	return normalRenderingBufferSize;
}
