#include <set>
#include <FireCube.h>
using namespace FireCube;
#include "Document.h"

Document::Document() : modelNode(nullptr), gridNode(nullptr), model(nullptr)
{

}

void Document::CreateRootNode(Engine *engine)
{
	root = NodePtr(new Node(engine, "Root"));	
	
	gridMaterial = MaterialPtr(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	gridMaterial->SetTechnique(engine->GetResourcePool()->GetResource<Technique>("Techniques/Unlit.xml"));
}

bool Document::Load(const std::string &filename, Engine *engine)
{	
	if (!modelNode)
		modelNode = root->CreateChild("Model").get();
	root->RemoveAllComponents();	
	model = root->CreateComponent<StaticModel>();
	model->CreateFromMesh(engine->GetResourcePool()->GetResource<Mesh>(filename));
	
	//modelNode->RemoveAllChildren();
	//modelNode->Load(filename);
		
	verticesCount = 0;
	facesCount = 0;
	CountElements(verticesCount, facesCount);
	/*normalRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	tangentRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	bitangentRenderingBuffer = FireCube::BufferPtr(new FireCube::Buffer);
	normalRenderingBuffer->Create();
	tangentRenderingBuffer->Create();
	bitangentRenderingBuffer->Create();	*/
	
	return true;
}
NodePtr Document::GetRoot()
{
	return root;
}
void Document::CountElements(unsigned int &verticesCount, unsigned int &facesCount)
{
	if (model)
	{	
		std::set<VertexBufferPtr> buffers;
		for (auto g : model->GetGeometries())
		{
			buffers.insert(g->GetVertexBuffer());
			facesCount += g->GetPrimitiveCount();
		}
		for (auto buffer : buffers)
		{
			verticesCount += buffer->GetVertexCount();
		}
	}
}
void Document::GenerateNormals(float l)
{
	/*vector<FireCube::vec3> normals;
	GenerateNormals(root, l, normals);
	normalRenderingBuffer->LoadData(&normals[0], normals.size()*sizeof(FireCube::vec3), FireCube::STATIC);
	normalRenderingBufferSize = normals.size();*/
}
void Document::GenerateNormals(NodePtr node, float l, std::vector<vec3> &normals)
{
	/*if (node->GetType() == FireCube::Node::GEOMETRY)
	{
		FireCube::GeometryNodePtr geometryNode = dynamic_pointer_cast<FireCube::GeometryNode>(node);
		if (geometryNode->GetGeometry())
		{
			for (unsigned int i = 0; i < geometryNode->GetGeometry()->GetVertices().size(); i++)
			{
				normals.push_back(geometryNode->GetGeometry()->GetVertices()[i]);
				normals.push_back(geometryNode->GetGeometry()->GetVertices()[i] + geometryNode->GetGeometry()->GetNormals()[i]*l);
			}
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateNormals(*i, l, normals);*/
}
void Document::GenerateTangents(float l)
{
	/*vector<FireCube::vec3> tangents;
	GenerateTangents(root, l, tangents);
	if (!tangents.empty())
	{
		tangentRenderingBuffer->LoadData(&tangents[0], tangents.size()*sizeof(FireCube::vec3), FireCube::STATIC);
		hasTangents = true;
	}
	else
		hasTangents = false;*/
}
void Document::GenerateTangents(NodePtr node, float l,std::vector<vec3> &tangents)
{
	/*if (node->GetType() == FireCube::Node::GEOMETRY)
	{
		FireCube::GeometryNodePtr geometryNode = dynamic_pointer_cast<FireCube::GeometryNode>(node);
		if (geometryNode->GetGeometry() && !geometryNode->GetGeometry()->GetTangents().empty())
		{
			for (unsigned int i = 0; i < geometryNode->GetGeometry()->GetVertices().size(); i++)
			{
				tangents.push_back(geometryNode->GetGeometry()->GetVertices()[i]);
				tangents.push_back(geometryNode->GetGeometry()->GetVertices()[i] + geometryNode->GetGeometry()->GetTangents()[i]*l);
			}
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateTangents(*i, l, tangents);*/
}
void Document::GenerateBitangents(float l)
{
	/*vector<FireCube::vec3> bitangents;
	GenerateBitangents(root, l, bitangents);
	if (!bitangents.empty())
		bitangentRenderingBuffer->LoadData(&bitangents[0], bitangents.size()*sizeof(FireCube::vec3), FireCube::STATIC);*/
}
void Document::GenerateBitangents(NodePtr node, float l, std::vector<vec3> &bitangents)
{
	/*if (node->GetType() == FireCube::Node::GEOMETRY)
	{
		FireCube::GeometryNodePtr geometryNode = dynamic_pointer_cast<FireCube::GeometryNode>(node);
		if (geometryNode->GetGeometry() && !geometryNode->GetGeometry()->GetBitangents().empty())
		{
			for (unsigned int i = 0; i < geometryNode->GetGeometry()->GetVertices().size(); i++)
			{
				bitangents.push_back(geometryNode->GetGeometry()->GetVertices()[i]);
				bitangents.push_back(geometryNode->GetGeometry()->GetVertices()[i] + geometryNode->GetGeometry()->GetBitangents()[i]*l);
			}
		}
	}
	for (vector<FireCube::NodePtr>::iterator i = node->GetChildren().begin(); i != node->GetChildren().end(); i++)
		GenerateBitangents(*i, l, bitangents);*/
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
/*FireCube::BufferPtr Document::GetNormalRenderingBuffer()
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
}*/

int Document::GetNormalRenderingBufferSize()
{
	return normalRenderingBufferSize;
}

std::vector<FireCube::MaterialPtr> Document::GetAllMaterials()
{		
	if (model)
		return model->GetMaterials();
	else
		return std::vector<MaterialPtr>();
}

void Document::CreateGrid(float size, DWORD numberOfCells)
{
	if (!gridNode)
		gridNode = root->CreateChild("GridNode").get();	
	gridNode->RemoveAllComponents();
	gridGeometry = gridNode->CreateComponent<CustomGeometry>();	
	for (DWORD i = 0; i < numberOfCells + 1; i++)
	{
		vec3 pos1((float)i * size - size * (float)numberOfCells / 2.0f, 0, -size * (float)numberOfCells / 2.0f);
		vec3 pos2((float)i * size - size * (float)numberOfCells / 2.0f, 0, size * (float)numberOfCells / 2.0f);
		vec3 pos3(-size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		vec3 pos4(size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		gridGeometry->AddVertex(pos1);
		gridGeometry->AddVertex(pos2);
		gridGeometry->AddVertex(pos3);
		gridGeometry->AddVertex(pos4);
	}
	gridGeometry->SetPrimitiveType(LINES);
	gridGeometry->SetMaterial(gridMaterial);
	gridGeometry->UpdateGeometry();
}